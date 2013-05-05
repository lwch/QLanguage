/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   20:35
	filename: 	\QLanguage\Parser\Parser.h
	file path:	\QLanguage\Parser
	file base:	Parser
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_PARSER_H_
#define _QLANGUAGE_PARSER_H_

#include "BasicParser.h"

namespace QLanguage
{
    class Parser : public BasicParser
    {
    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);
    };
}

#endif
