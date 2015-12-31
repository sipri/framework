#if !defined TREE_H
#define TREE_H
//------------------------------------
//  tree.h
//  (c) Bartosz Milewski, 1994
//------------------------------------

class Node
{
public:
    virtual ~Node () {}
    virtual double Calc () const = 0;
    virtual bool IsLvalue () const { return false; }
    virtual void Assign (double value) {}
};

class NumNode: public Node
{
public:
    NumNode ( double num ) : _num (num ) {}
    double Calc () const;
private:
    const double _num;
};

class UMinusNode: public Node
{
public:
    UMinusNode (Node * pNode) : _pNode (pNode) {}
    ~UMinusNode () { delete _pNode; }
    double Calc () const;
private:
    Node * const _pNode;
};

typedef double (*PFun)(double); 

class FunNode: public Node
{
public:
    FunNode (PFun pFun, Node * pNode) : _pNode (pNode), _pFun (pFun) {}
    ~FunNode () { delete _pNode; }
    double Calc () const;
private:
    Node * const _pNode;
    PFun         _pFun;
};

class Store;

class VarNode: public Node
{
public:
    VarNode (int id, Store & store)
        : _id (id), _store(store) {}
    double Calc () const;
    bool IsLvalue () const;
    void Assign (double val );
private:
    const int _id;
    Store & _store;
};

class BinNode: public Node
{
public:
    BinNode (Node * pLeft, Node * pRight)
    : _pLeft (pLeft), _pRight (pRight) {}
    ~BinNode ();
protected: 
    Node * const _pLeft;
    Node * const _pRight;
};

class AddNode: public BinNode
{
public:
    AddNode (Node * pLeft, Node * pRight)
        : BinNode (pLeft, pRight) {}
    double Calc () const;
};

class SubNode: public BinNode
{
public:
    SubNode (Node * pLeft, Node * pRight)
        : BinNode (pLeft, pRight) {}
    double Calc () const;
};

class MultNode: public BinNode
{
public:
    MultNode (Node * pLeft, Node * pRight)
        : BinNode (pLeft, pRight) {}
    double Calc () const;
};

class DivideNode: public BinNode
{
public:
    DivideNode (Node * pLeft, Node * pRight)
        : BinNode (pLeft, pRight) {}
    double Calc () const;
};

class AssignNode : public BinNode
{
public:
    AssignNode (Node * pLeft, Node * pRight)
        : BinNode (pLeft, pRight) {}
    double Calc () const;
};

#endif 
