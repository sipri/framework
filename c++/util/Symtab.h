#if !defined SYMTAB_H
#define SYMTAB_H
//------------------------------------
//  symtab.h
//  (c) Bartosz Milewski, 1994
//------------------------------------
#include <cstring>
#include <cassert>

const int maxIdFun = 16;

typedef double (*PFun) (double);

class FunctionEntry
{
public:
    PFun pFun;
    char* strFun;
};

extern FunctionEntry funArr [];

class SymbolTable;

class FunctionTable
{
public:
    FunctionTable (SymbolTable& symTab, FunctionEntry funArr []);
    int Size () const { return _size; }
    PFun GetFun (int id) { return _pFun [id]; }
private:
    PFun  _pFun [maxIdFun];
    int   _size;
};

class Link;

class LinkAllocator
{
	enum { BlockLinks = 16 };
	class Block
	{
	public:
		Block (Block * next) : _next (next) {}
		Block * Next () { return _next; }
		void SetNext (Block * next) { _next = next; }
	private:
		Block * _next;
	};
public:
	LinkAllocator () : _p (0), _blocks (0) {}
	~LinkAllocator ();
	void Purge ();
	void * NewLink ();
	void Recycle (void * link);
private:
	Link  * _p;
	Block * _blocks;
};

// stores integer id in a link
class Link
{
	friend class LinkAllocator;
public:
    Link (Link * pNext, int id)
    : _pNext (pNext), _id (id) {}

    Link *  Next () const { return _pNext; }
    int     Id () const { return _id; }
	// allocator
	void * operator new (size_t size)
	{
		assert (size == sizeof (Link));
		return _alloc.NewLink ();
	}
	void operator delete (void * mem)
	{
		_alloc.Recycle (mem);
	}
	static void Purge () { _alloc.Purge (); }
private:
	static	LinkAllocator _alloc;

    Link *  _pNext;
    int     _id;
};

// Linked list of id's stored in links

class List
{
public:
    List ();
    ~List ();
    void Add (int id);
    Link const * GetHead () const { return _pHead; }
private:
    Link * _pHead;
};

// Hash table of strings

class HTable
{
public:
    explicit HTable (int size): _size(size)
    {
        _aList = new List [size];
    }

    ~HTable ()
    {
        delete [] _aList;
		// release memory in free list
		Link::Purge ();
    }

    List const & Find (char const * str, int len) const;
    List const & Find (char const * str) const
	{
		return Find (str, strlen (str));
	}
    void Add (char const * str, int len, int id);
    void Add (char const * str, int id)
	{
		Add (str, strlen (str), id);
	}
private:
    int hash (char const * str, int len) const;

    List * _aList;
    int    _size;
};

const int idNotFound = -1;

// String table maps strings to ints
// and ints to strings

class SymbolTable
{
public:
    explicit SymbolTable (int size);
    ~SymbolTable ();
    int ForceAdd (char const * str, int len);
    int Find (char const * str, int len) const;
    char const * GetString (int id) const;
private:
    HTable  _htab;
    int *   _offStr; // offsets of strings in buffer
    int     _size;
    int     _curId;
    char *  _strBuf;
    int     _bufSize;
    int     _curStrOff;
};
#endif
