/********************************************************************
	created:	2013/05/06
	created:	6:5:2013   22:57
	filename: 	\QLanguage\Parser\ParserTable.h
	file path:	\QLanguage\Parser
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
#include "../../QLanguage/Lexer/Lexer.h"
#include "../../QLanguage/Parser/BasicParser.h"

namespace QLanguage
{
    using namespace Library;

    class ParserTable
    {
    public:
        ParserTable();

        bool loadFromData(const char* data, size_t size);
        bool parse(const list<Lexer::Token>& l, BasicParser* pParser);
    protected:
        const bool compareString(const char* data, size_t size, const char* compare)const;
        long index_of_vt(const string& str, long idx = 0);
        long getGoTo(ushort s, const Production::Item& i);
    public:
        vector<Production>           rules;
    protected:
        Rule::Context                ruleContext;
        vector<pair<uchar, ushort> > table;
        vector<Production::Item>     vts;
        vector<Production::Item>     vns;
        vector<uint>                 items;
        uint                         iStart;
    };
}

#endif
