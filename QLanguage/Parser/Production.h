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
    class LALR1Production;

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

            Item();
            Item(Rule::Context* pContext);
            Item(const string& name);
            Item(const Rule& rule);
            Item(const Item& i);

            Item& operator=(const Item& i);

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
                return (index != x.index || type != x.type) || (type == TerminalSymbol ? rule != x.rule : false);
            }

            inline const bool isNoTerminalSymbol()const
            {
                return type == NoTerminalSymbol;
            }

            inline const bool isTermainalSymbol()const
            {
                return type == TerminalSymbol;
            }

            static uint inc();

            bool loadFromData(const char*& data);
            bool loadFromStream(istream& stream);

            const bool output(ostream& stream)const;
        };
    public:
        Production();
        Production(const Item& left);
        Production(const Item& left, const Item& item);
        Production(const Item& left, const vector<Item>& right);
        Production(const Production& p);

        const bool operator==(const LALR1Production& p)const;

        inline const bool operator<(const Production& p)const
        {
            return index < p.index;
        }

        void print(Library::ostream& stream)const;

        bool loadFromData(const char*& data, Rule::Context* pContext);
        bool loadFromStream(istream& stream, Rule::Context* pContext);

        const bool output(Library::ostream& stream)const;
    protected:
        static uint inc();
    public:
        Item left;
        vector<Item> right;
        uint index;
    };
}

#endif
