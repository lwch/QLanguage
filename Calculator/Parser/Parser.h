/********************************************************************
	created:	2013/05/10
	created:	10:5:2013   12:54
	filename: 	\Calculator\Parser\Parser.h
	file path:	\Calculator\Parser
	file base:	Parser
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _PARSER_H_
#define _PARSER_H_

#include "../../QLanguage/Parser/BasicParser.h"

namespace QLanguage
{
    class Parser : public BasicParser
    {
    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);

        bool reduceReal();
        bool reduceDigit();
        bool reduceAdd();
        bool reduceSub();
        bool reduceMul();
        bool reduceDiv();
        bool reduceBrackets();
        bool reduceSin();
        bool reduceCos();
        bool reduceLog();
        bool reduceSymValue();
    protected:
        vector<string> shifts;
        stack<double>  numbers;
    };
}

#endif
