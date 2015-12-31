//------------------------------------
//  tree.cpp
//  (c) Bartosz Milewski, 1994
//------------------------------------

#include "tree.h"
#include "store.h"
#include <cmath>
#include <iostream>
using std::cout;

BinNode::~BinNode ()
{
    delete _pLeft;
    delete _pRight;
}

double NumNode::Calc () const
{
    return _num;
}

double VarNode::Calc () const
{
	double x = 0.0;
    if (_store.IsInit (_id ))
        x = _store.Value (_id);
	else
		cout << "Use of uninitialized variable\n";
    return x;
}

void VarNode::Assign (double val)
{
    _store.SetValue (_id, val);
}

bool VarNode::IsLvalue () const
{
    return true;
}

double UMinusNode::Calc () const
{
    return - _pNode->Calc();
}

double FunNode::Calc () const
{
    double val = 0;
    assert (_pFun != 0);
    val = (*_pFun)(_pNode->Calc());
    return val;
}

double AddNode::Calc () const
{
    return _pLeft->Calc () + _pRight->Calc ();
}

double SubNode::Calc () const
{
    return _pLeft->Calc () - _pRight->Calc ();
}

double MultNode::Calc () const
{
    return _pLeft->Calc () * _pRight->Calc ();
}

double DivideNode::Calc () const
{
    double x = _pRight->Calc ();
    if (x == 0.0)
    {
        cout << "Division by zero!\n";
        return HUGE_VAL;
    }
    return _pLeft->Calc () / x;
}

double AssignNode::Calc () const
{
    double x = _pRight->Calc ();
    _pLeft->Assign (x);
    return x;
}
