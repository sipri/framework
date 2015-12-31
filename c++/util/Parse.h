#if !defined PARSE_H
#define PARSE_H
//------------------------------------
//  parse.h
//  (c) Bartosz Milewski, 1994
//------------------------------------

#include "tree.h"

enum Status
{
    stOk,
    stQuit,
    stError
};

class Node; // forward declaration
class Scanner;
class Store;
class FunctionTable;
class SymbolTable;

class Parser
{
public:
    Parser (Scanner & scanner,
        Store & store,
        FunctionTable & funTab,
        SymbolTable & symTab );
    ~Parser ();
    Status Eval ();
private:
    void Parse();
    Node * Expr();
    Node * Term();
    Node * Factor();
    void Execute ();

    Scanner &    _scanner;
    Node *       _pTree;
    Status       _status;
    Store &      _store;
    FunctionTable &  _funTab;
    SymbolTable &    _symTab;
};
#endif
