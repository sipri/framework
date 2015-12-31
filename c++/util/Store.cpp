//------------------------------------
//  store.cpp
//  (c) Bartosz Milewski, 1994
//------------------------------------

#include "store.h"
#include "symtab.h"
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

Store::Store (int size, SymbolTable & symTab): _size (size)
{
    _cell = new double [size];
    _status = new unsigned char [size];
    for (int i = 0; i < size; ++i)
        _status [i] = stNotInit;

    // add predefined constants
    // Note: if more needed, do a more general job
    cout << "e = " << exp (1) << endl;
    int id = symTab.ForceAdd ("e", 1);
    SetValue (id, exp (1));
    cout << "pi = " << 2 * acos (0.0) << endl;
    id = symTab.ForceAdd ("pi", 2);
    SetValue (id, 2 * acos (0.0));
}
