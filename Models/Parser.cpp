#include "Parser.h"

namespace QLanguage
{
    Parser::Parser(const vector<Production>& productions) : BasicParser(productions)
    {
    }

    Parser::~Parser()
    {
    }

    bool Parser::shift(const string& s)
    {
        shifts.push_back(s);
        return true;
    }

    bool Parser::reduce(ushort i)
    {
        return true;
    }
}