/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   20:20
	filename: 	\QLanguage\Parser\BasicParser.cpp
	file path:	\QLanguage\Parser
	file base:	BasicParser
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "BasicParser.h"

namespace QLanguage
{
    BasicParser::BasicParser(const vector<Production>& productions) : inputProductions(productions)
    {
    }

    BasicParser::~BasicParser()
    {
    }
}
