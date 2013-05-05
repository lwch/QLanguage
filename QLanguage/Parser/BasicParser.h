/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   20:19
	filename: 	\QLanguage\Parser\BasicParser.h
	file path:	\QLanguage\Parser
	file base:	BasicParser
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_BASIC_PARSER_H_
#define _QLANGUAGE_BASIC_PARSER_H_

#include "Production.h"
#include "../../QCore/Library/vector.h"

namespace QLanguage
{
    class BasicParser
    {
    public:
        BasicParser(const vector<Production>& productions);
        virtual ~BasicParser();

        virtual bool shift(const string& s)=0;
        virtual bool reduce(ushort i)=0;
    protected:
        vector<Production> inputProductions;
    };
}

#endif
