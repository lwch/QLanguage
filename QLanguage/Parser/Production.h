/********************************************************************
	created:	2013/03/09
	created:	9:3:2013   14:28
	filename: 	\QLanguage\Parser\Production.h
	file path:	\QLanguage\Parser
	file base:	Production
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_PRODUCTION_H_
#define _QLANGUAGE_PRODUCTION_H_

#include <assert.h>

#include "../../QCore/Library/list.h"
#include "../../QCore/Library/regex/regex.h"

using namespace QLanguage::Library;
using namespace QLanguage::Library::regex;

namespace QLanguage
{
    class Production
    {
    public:
        struct Item
        {
            enum Type
            {
                TerminalSymbol,
                NoTerminalSymbol,
            }type;

            Rule rule;

            Item() : type(NoTerminalSymbol) {}
            Item(const Rule& rule) : type(TerminalSymbol), rule(rule) {}
        };
    public:
        Production(const Item& left) : left(left) {}
        Production(const Item& left, const list<Item>& right) : left(left), right(right) {}
        Production(const Production& p) : left(p.left), right(p.right) {}
    public:
        Item left;
        list<Item> right;
    };
}

#endif
