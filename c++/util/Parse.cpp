//------------------------------------
//  parse.cpp
//  (c) Bartosz Milewski, 1994
//------------------------------------

#include "parse.h"
#include "scan.h"
#include "tree.h"
#include "symtab.h"
#include "store.h"

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

Parser::Parser (Scanner & scanner, 
        Store & store,
        FunctionTable & funTab,
        SymbolTable & symTab )
: _scanner (scanner), 
  _pTree (0), 
  _status (stOk), 
  _funTab (funTab),
  _store (store),
  _symTab (symTab)
{
}

Parser::~Parser ()
{
    delete _pTree;
}

Status Parser::Eval ()
{
    Parse ();
    if (_status == stOk)
        Execute ();
    else
        _status = stQuit;
    return _status;
}

void Parser::Execute ()
{
    if (_pTree)
    {
        double result = _pTree->Calc ();
        cout << "  " << result << endl;
    }
}

void Parser::Parse ()
{
    _pTree = Expr ();
}

Node * Parser::Expr()
{
    Node * pNode = Term ();
    EToken token = _scanner.Token ();
    if (token == tPlus)
    {
        _scanner.Accept ();
        Node * pRight = Expr ();
        pNode = new AddNode (pNode, pRight);
    }
    else if (token == tMinus)
    {
        _scanner.Accept ();
        Node * pRight = Expr ();
        pNode = new SubNode (pNode, pRight);
    }
    else if (token == tAssign)
    {
        _scanner.Accept ();
        Node * pRight = Expr ();
        if (pNode->IsLvalue ())
        {
            pNode = new AssignNode (pNode, pRight);
        }
        else
        {
            _status = stError;
            delete pNode;
            pNode = Expr ();
        }
    }
    return pNode;
}

Node * Parser::Term ()
{
    Node * pNode = Factor ();
    if (_scanner.Token () == tMult)
    {
        _scanner.Accept ();
        Node * pRight = Term ();
        pNode = new MultNode (pNode, pRight);
    }
    else if (_scanner.Token() == tDivide)
    {
        _scanner.Accept ();
        Node * pRight = Term ();
        pNode = new DivideNode (pNode, pRight);
    }
    return pNode;
}

Node * Parser::Factor ()
{
    Node * pNode;
    EToken token = _scanner.Token ();

    if (token == tLParen)
    {
        _scanner.Accept (); // accept '('
        pNode = Expr ();
        if (_scanner.Token () != tRParen)
            _status = stError;
        _scanner.Accept (); // accept ')'
    }
    else if (token == tNumber)
    {
        pNode = new NumNode (_scanner.Number ());
        _scanner.Accept ();
    }
    else if (token == tIdent)
    {
        char strSymbol [maxSymLen];
        int lenSym = maxSymLen;
        // copy the symbol into strSymbol
        _scanner.GetSymbolName (strSymbol, lenSym);
        int id = _symTab.Find (strSymbol, lenSym);
        _scanner.Accept ();
        if (_scanner.Token () == tLParen) // function call
        {
            _scanner.Accept (); // accept '('
            pNode = Expr ();
            if (_scanner.Token () == tRParen)
	            _scanner.Accept (); // accept ')'
			else
                _status = stError;
            if (id != idNotFound && id < _funTab.Size ())
            {
                pNode = new FunNode (
                    _funTab.GetFun (id), pNode);
            }
            else
            {
                cout << "Unknown function \"";
                cout << strSymbol << "\"" << endl;
            }
        }
        else
        {
            if (id == idNotFound)
                id = _symTab.ForceAdd (strSymbol, lenSym);
            pNode = new VarNode (id, _store);
        }
    }
    else if (token == tMinus) // unary minus
    {
        _scanner.Accept (); // accept minus
        pNode = new UMinusNode (Factor ());
    }
    else
    {
        _scanner.Accept ();
        _status = stError;
        pNode = 0;
    }
    return pNode;
}

const int maxBuf = 100;
const int maxSymbols = 40;

int main ()
{
    // Notice all these local objects.
    // A clear sign that there should be
    // a top level object, say, the Calculator.
    // Back to the drawing board!

    char buf [maxBuf];
    Status status;
    SymbolTable symTab (maxSymbols);
    FunctionTable funTab (symTab, funArr);
    Store store (maxSymbols, symTab);

    do
    {
        cout << "> ";  // prompt
        cin.getline (buf, maxBuf);
        Scanner scanner (buf);
        Parser  parser (scanner, store, funTab, symTab);
		Parser parser2 (parser);
        status = parser.Eval ();
    } while (status != stQuit);
	return 0;
}
