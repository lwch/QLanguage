/********************************************************************
	created:	2013/04/01
	created:	1:4:2013   17:08
	filename: 	\QLanguage\Parser\LALR1.h
	file path:	\QLanguage\Parser
	file base:	LALR1
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LALR1_H_
#define _QLANGUAGE_LALR1_H_

#include "LR0.h"
#include "LRProduction.h"

namespace QLanguage
{
    class LALR1
    {
        class Item
        {
        public:
            map<Production::Item, vector<LALR1Production> > data;
#ifdef _DEBUG
            uint idx;

            Item(const LR0::Item& x) : idx(x.idx) { createFromLR0Item(x); }
#else
            Item(const LR0::Item& x) { createFromLR0Item(x); }
#endif

            inline const bool operator==(const Item& x)const
            {
                return data == x.data;
            }

            inline const bool operator==(const LR0::Item& x)const
            {
                return data == x.data;
            }
        protected:
            void createFromLR0Item(const LR0::Item& x)
            {
                data.clear();
                for (map<LR0Production::Item, vector<LR0Production> >::const_iterator i = x.data.begin(), m = x.data.end(); i != m; ++i)
                {
                    for (vector<LR0Production>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                    {
                        data[i->first].push_back(*j);
                    }
                }
            }
        };

        struct Edge 
        {
            Item* pFrom;
            Item* pTo;
            Production::Item item;

            Edge(Item* pFrom, Item* pTo, const Production::Item& item) : pFrom(pFrom), pTo(pTo), item(item) {}

            inline const bool operator==(const Edge& x)const
            {
                return pFrom == x.pFrom && pTo == x.pTo && item == x.item;
            }
        };

        class Context
        {
        public:
            set<Item*> states;

            void clear()
            {
                for (set<Item*>::iterator i = states.begin(), m = states.end(); i != m; ++i)
                {
                    destruct(*i, has_destruct(*(*i)));
                    Item_Alloc::deallocate(*i);
                }
            }

            ~Context()
            {
                clear();
            }
        };

        typedef allocator<Item> Item_Alloc;
    public:
        LALR1(LR0& lr0);

        bool make();

        void print();
    protected:
        void closure(Item* pItem, bool& bContinue);
        Item* go(Item* pItem, const Production::Item& x);
        void afirst(const LALR1Production& p, vector<LALR1Production::Item>& v);
        void appendWildCards(Item* pItem, const Production::Item& left, const vector<LALR1Production::Item>& v, vector<Production::Item>& vts, bool& bContinue);
        void spreadWildCards(Item* pItem, const vector<LALR1Production::Item>& v, bool& bContinue);
    protected:
        LR0& lr0;

        Item*      pStart;
        set<Item*> pEnd;
        hashmap<Item*, vector<Edge> > edges;
        vector<Item*> items;

        Context context;
    };
}

#endif
