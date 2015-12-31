#if !defined SCAN_H
#define SCAN_H
//------------------------------------
//  scan.h
//  (c) Bartosz Milewski, 1994
//------------------------------------

#include <cassert>

enum EToken
{
    tEnd,
    tError,
    tNumber,
    tPlus,
    tMult,
    tMinus,
    tDivide,
    tLParen,
    tRParen,
    tAssign,
    tIdent
};

const int maxSymLen = 80;

class Scanner
{
public:
    Scanner (char const * buf);
    EToken  Token () { return _token; }
    EToken  Accept ();
    void GetSymbolName (char * strOut, int & len);
    double Number ()
    {
        assert (_token == tNumber);
        return _number;
    }
private:
    void EatWhite ();

    char const * const  _buf;
    int                 _iLook;
    EToken              _token;
    double              _number;
    int                 _iSymbol;
    int                 _lenSymbol;
};
#endif
