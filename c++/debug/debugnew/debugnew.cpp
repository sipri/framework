#include "debugnew.h"
#include <sstream>
#include <new>
#include <windows.h>

Tracer NewTrace;

bool Tracer::Ready = false;

void * operator new (size_t size, char const * file, int line)
{
	void * p = malloc (size);
	if (Tracer::Ready)
		NewTrace.Add (p, file, line, size);
	return p;
}

void operator delete (void * p, char const * file, int line)
{
	if (Tracer::Ready)
		NewTrace.Remove (p);
	free (p);
}

void * operator new (size_t size)
{
	void * p = malloc (size);
	if (Tracer::Ready)
		NewTrace.Add (p, "?", 0, size);
	return p;
}

void operator delete (void * p)
{
	if (Tracer::Ready)
		NewTrace.Remove (p);
	free (p);
}

Tracer::Tracer () 
: _lockCount (0) 
{
	Ready = true;
}

Tracer::~Tracer ()
{
	Ready = false;
	Dump ();
}

void Tracer::Add (void * p, char const * file, int line, size_t size)
{
	if (_lockCount > 0)
		return;
	Tracer::Lock lock (*this);
	_map [p] = Entry (file, line, size);
}

void Tracer::Remove (void * p)
{
	if (_lockCount > 0)
		return;

	Tracer::Lock lock (*this);

	iterator it = _map.find (p);
	if (it != _map.end ())
	{
		char const * file = it->second.File ();
		int line = it->second.Line ();
		_map.erase (it);
	}
}

void Tracer::Dump ()
{
	if (_map.size () != 0)
	{
		for (iterator it = _map.begin (); it != _map.end (); ++it)
		{
			char const * file = it->second.File ();
			int line = it->second.Line ();
			size_t size = it->second.Size ();
			int addr = reinterpret_cast<int> (it->first);
			std::stringstream out;
			char buffer1 [10];
			char buffer2 [8];
			char buffer3 [64];
			out << "*** Memory leak : 0x" << itoa (addr, buffer1, 16) << ": "
				<< file
				<< ", line " << itoa (line, buffer2, 10)
				<< ", size " << itoa (size, buffer3, 10) << std::endl;
			::OutputDebugString (out.str ().c_str ());
		}
	}
}
