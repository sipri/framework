//------------------------------------
//  symtab.cpp
//  (c) Bartosz Milewski, 1994
//------------------------------------

#include "symtab.h"

#include <cassert>
#include <cstring>
#include <cmath>
#include <iostream>
#include <new> // for placement new
using std::cout;
using std::endl;

double CoTan (double x) 
{
    double y = tan (x);
    if (y == 0)
    {
        cout << "cotan of " << x << " undefined\n";
        return HUGE_VAL;
    }
    return 1.0 / y;
}

FunctionEntry funArr [maxIdFun] =
{ 
    log,  "log",
    log10,"log10",
    exp,  "exp",
    sqrt, "sqrt",
    sin,  "sin",
    cos,  "cos",
    tan,  "tan",
    CoTan,"cotan",
    sinh, "sinh",
    cosh, "cosh",
    tanh, "tanh",
    asin, "asin",
    acos, "acos",
    atan, "atan",
    0,    ""
};

FunctionTable::FunctionTable (SymbolTable & symTab, FunctionEntry funArr [])
    : _size(0)
{
    for (int i = 0; i < maxIdFun; ++i)
    {
        int len =  strlen (funArr [i].strFun);
        if (len == 0)
            break;
        _pFun [i] = funArr [i].pFun;
        cout << funArr[i].strFun << endl;
        int j = symTab.ForceAdd (funArr[i].strFun, len);
        assert (i == j);
        ++_size;
    }
}

// Link Allocator

LinkAllocator Link::_alloc;

void * LinkAllocator::NewLink ()
{
	if (_p == 0)
	{
		// use global operator new to allocate a block of links
		char * p = ::new char [sizeof (Block) + BlockLinks * sizeof (Link)];
		// add it to the list of blocks
		//Block * block = reinterpret_cast<Block *> (p);
		//block->SetNext (_blocks);
		// use placement new instead
		Block * block = new (p) Block (_blocks);
		_blocks = block;
		// add it to the list of links
		p += sizeof (Block);
		for (int i = 0; i < BlockLinks; ++i)
		{
			Link * link = reinterpret_cast<Link *> (p);
			link->_pNext = _p;
			_p = link;
			p += sizeof (Link);
		}
	}
	void * mem = _p;
	_p = _p->_pNext;
	return mem;
}

void LinkAllocator::Recycle (void * mem)
{
	Link * link = static_cast<Link *> (mem);
	link->_pNext = _p;
	_p = link;
}

LinkAllocator::~LinkAllocator ()
{
	Purge ();
}

void LinkAllocator::Purge ()
{
    while (_blocks != 0)
    {
		// it was allocated as an array of char
        char * mem = reinterpret_cast<char *> (_blocks);
        _blocks = _blocks->Next();
        ::delete [] mem;
    }
}

List::List ()
    : _pHead(0)
{}

List::~List ()
{
    // free linked list
    while (_pHead != 0)
    {
        Link * pLink = _pHead;
        _pHead = _pHead->Next();
        delete pLink;
    }
}

void List::Add (int id)
{
    // add in front of the list
    Link * pLink = new Link (_pHead, id);
    _pHead = pLink;
}

// Find the list in hash table that may contain
// the id of the string we are looking for

List const & HTable::Find (char const * str, int len) const
{
    int i = hash (str, len);
    assert (i >= 0 && i < _size);
    return _aList [i];
}

void HTable::Add (char const * str, int len, int id)
{
    int i = hash (str, len);
    assert (i >= 0 && i < _size);
    _aList [i].Add (id);
}

// Private hashing function

int HTable::hash (char const * str, int len) const
{
    // no empty strings, please
    assert (len != 0);
    // must be unsigned, hash should return positive number
    unsigned h = str [0];
    for (int i = 1; i < len; ++i)
        h = (h << 4) + str [i];
    return h % _size;  // small positive integer
}

SymbolTable::SymbolTable (int size)
    : _size (size), _curId (0), _curStrOff (0), _htab (size + 1)
{
    _offStr = new int [size];
    _bufSize = size * 10;
    _strBuf = new char [_bufSize];
}

SymbolTable::~SymbolTable()
{
    delete []_offStr;
    delete []_strBuf;
}

// Add string without looking for duplicates

int SymbolTable::ForceAdd (char const * str, int len)
{
    // is there enough space?
    if (_curId == _size
        || _curStrOff + len + 1 >= _bufSize)
    {
        return idNotFound;
    }
    // point to place where the string will be stored
    _offStr [_curId] = _curStrOff;
    // copy the string there
    strncpy (&_strBuf [_curStrOff], str, len);
    // calculate new offset
    _curStrOff += len;
    _strBuf [_curStrOff] = 0;  // null terminate
    ++_curStrOff;
    // add to hash table
    _htab.Add (str, len, _curId);
    ++_curId;
    return _curId - 1;
}

int SymbolTable::Find (char const * str, int len) const
{
    // Get a short list from hash table
    List const & list = _htab.Find (str);
    // Iterate over this list
    for (Link const * pLink = list.GetHead ();
        pLink != 0;
        pLink = pLink->Next () )
    {
        int id = pLink->Id ();
		int offStr = _offStr [id];
        // char const * strStored = &_strBuf [ offStr ];
        char const * strStored = _strBuf + offStr;
        if (strcmp (str, strStored) == 0) // they're equal
        {
            return id;  // success!
        }
    }
    return idNotFound;
}

// map integer into string. Must be valid id

char const * SymbolTable::GetString (int id) const
{
    assert (id >= 0);
    assert (id < _curId);
    int offStr = _offStr [id];
    return &_strBuf [offStr];
    // return _strBuf + offStr;
}
