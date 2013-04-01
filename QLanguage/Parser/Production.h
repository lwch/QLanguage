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

#include "../../QCore/Library/vector.h"
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
            uint index;

            Item() : type(NoTerminalSymbol) { index = inc(); }
            Item(const Rule& rule) : type(TerminalSymbol), rule(rule) { index = inc(); }
            Item(const Item& i) : type(i.type), rule(i.rule), index(i.index) {}

            inline Item& operator=(const Item& i)
            {
                if (&i != this)
                {
                    type  = i.type;
                    rule  = i.rule;
                    index = i.index;
                }
                return *this;
            }

            inline const bool operator<(const Item& x)const
            {
                return index < x.index;
            }

            inline const bool operator==(const Item& x)const
            {
                return type == x.type && (type == TerminalSymbol ? rule == x.rule : true);
            }

            inline const bool operator!=(const Item& x)const
            {
                return type != x.type || (type == TerminalSymbol ? rule != x.rule : false);
            }

            static uint inc()
            {
                static uint i = 0;
                return i++;
            }
        };
    public:
        Production() {}
        Production(const Item& left) : left(left) {}
        Production(const Item& left, const Item& item) : left(left) { right.push_back(item); }
        Production(const Item& left, const vector<Item>& right) : left(left), right(right) {}
        Production(const Production& p) : left(p.left), right(p.right) {}
    public:
        Item left;
        vector<Item> right;
    };
}

#endif
