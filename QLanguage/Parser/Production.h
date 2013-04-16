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
#include "../../QCore/Library/fstream.h"

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
            string name;

            Item() {}
            Item(const string& name) : type(NoTerminalSymbol), index(inc()), name(name) {}
            Item(const Item& i) : type(i.type), rule(i.rule), index(i.index), name(i.name) {}
            Item(const Rule& rule) : type(TerminalSymbol), rule(rule), index(inc()) {}

            inline Item& operator=(const Item& i)
            {
                if (&i != this)
                {
                    type  = i.type;
                    rule  = i.rule;
                    index = i.index;
                    name  = i.name;
                }
                return *this;
            }

            inline const bool operator<(const Item& x)const
            {
                return index < x.index;
            }

            inline const bool operator==(const Item& x)const
            {
                return index == x.index && type == x.type && (type == TerminalSymbol ? rule == x.rule : true);
            }

            inline const bool operator!=(const Item& x)const
            {
                return (index != x.index && type != x.type) || (type == TerminalSymbol ? rule != x.rule : false);
            }

            static uint inc()
            {
                static uint i = 0;
                return i++;
            }
        };
    public:
        Production() {}
        Production(const Item& left) : left(left), index(inc()) {}
        Production(const Item& left, const Item& item) : left(left), index(inc()) { right.push_back(item); }
        Production(const Item& left, const vector<Item>& right) : left(left), right(right), index(inc()) {}
        Production(const Production& p) : left(p.left), right(p.right), index(inc()) {}

        inline const bool operator<(const Production& p)const
        {
            return index < p.index;
        }

        void print()const
        {
            printf("%s ->", left.name.c_str());
            for (vector<Item>::const_iterator i = right.begin(), m = right.end(); i != m; ++i)
            {
                if (i->type == Item::TerminalSymbol)
                {
                    printf(" ");
                    i->rule.printShowName();
                }
                else printf(" %s", i->name.c_str());
            }
            printf("\n");
        }

        void print(fstream& fs)const
        {
            fs << string::format("%s ->", left.name.c_str());
            for (vector<Item>::const_iterator i = right.begin(), m = right.end(); i != m; ++i)
            {
                if (i->type == Item::TerminalSymbol)
                {
                    fs << " ";
                    i->rule.printShowName(fs);
                }
                else fs << string::format(" %s", i->name.c_str());
            }
            fs << endl;
        }
    protected:
        static uint inc()
        {
            static uint i = 0;
            return i++;
        }
    public:
        Item left;
        vector<Item> right;
    protected:
        uint index;
    };
}

#endif
