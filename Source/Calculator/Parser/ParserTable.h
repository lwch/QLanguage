/********************************************************************
	created:	2013/05/06
	created:	6:5:2013   22:57
	filename: 	\Calculator\Parser\ParserTable.h
	file path:	\Calculator\Parser
	file base:	ParserTable
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _PARSER_TABLE_H_
#define _PARSER_TABLE_H_

#include "../../QCore/Library/typedef.h"
#include "../../QCore/Library/vector.h"
#include "../../QLanguage/Parser/Production.h"

namespace QLanguage
{
    using namespace Library;

    class ParserTable
    {
    public:
        ParserTable();

        bool loadFromData(const char* data, size_t size);
    protected:
        const bool compareString(const char* data, size_t size, const char* compare)const;
    protected:
        Rule::Context                ruleContext;
        vector<pair<uchar, ushort> > table;
        vector<Production::Item>     vts;
        vector<Production::Item>     vns;
    };
}

#endif
