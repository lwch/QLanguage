/********************************************************************
	created:	2013/03/10
	created:	10:3:2013   17:44
	filename: 	\QLanguage\Parser\LRProduction.h
	file path:	\QLanguage\Parser
	file base:	LRProduction
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LRPRODUCTION_H_
#define _QLANGUAGE_LRPRODUCTION_H_

#include "Production.h"

namespace QLanguage
{
    class LALR1Production : public Production
    {
        typedef Production parent;
    public:
        struct Item
        {
            enum { Rule, End }type;
            regex::Rule rule;

            Item();
            Item(const regex::Rule& rule);

            inline const bool operator==(const Item& x)const
            {
                return type == x.type && (type == End ? true : rule == x.rule);
            }

            inline const bool operator==(const Production::Item& x)const
            {
                return type == End ? false : rule == x.rule;
            }

            inline const bool operator!=(const Item& x)const
            {
                return type != x.type || (type == End ? true : rule != x.rule);
            }

            Item& operator=(const Item& x);
        };

        LALR1Production();
        LALR1Production(const Production::Item& left, const vector<Production::Item>& right);
        LALR1Production(const Production::Item& left, const Production::Item& right, size_t pos);
        LALR1Production(const Production& p, uint pos);
        LALR1Production(const LALR1Production& p);
        LALR1Production(const LALR1Production& p, uint pos);

        const bool operator==(const LALR1Production& p)const;
        const bool operator!=(const LALR1Production& p)const;

        LALR1Production& operator=(const LALR1Production& p);

        inline LALR1Production stepUp()
        {
            LALR1Production x(*this);
            ++x.idx;
            return x;
        }

        void print(Library::ostream& stream)const;
    public:
        uint         idx;
        vector<Item> wildCards;
    };
}

#endif
