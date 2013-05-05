/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   20:35
	filename: 	\QLanguage\Parser\Parser.cpp
	file path:	\QLanguage\Parser
	file base:	Parser
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
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
        return true;
    }

    bool Parser::reduce(ushort i)
    {
        return true;
    }
}