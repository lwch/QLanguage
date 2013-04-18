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
            vector<LALR1Production> data;
#ifdef _DEBUG
            uint idx;

            Item() : idx(inc()) {}
            Item(const LR0::Item& lr0Item) : idx(inc()) { createFromLR0(lr0Item); }
#else
            Item() {}
            Item(const LR0::Item& lr0Item) { createFromLR0(lr0Item); }
#endif

            inline const bool operator==(const Item& x)const
            {
                return data == x.data;
            }
        protected:
            static uint inc()
            {
                static uint i = 0;
                return i++;
            }

            void createFromLR0(const LR0::Item& lr0Item)
            {
                for (map<Production::Item, vector<LR0Production> >::const_iterator j = lr0Item.data.begin(), n = lr0Item.data.end(); j != n; ++j)
                {
                    for (vector<LR0Production>::const_iterator k = j->second.begin(), o = j->second.end(); k != o; ++k)
                    {
                        if (k->bKernel) data.push_back(*k);
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

            void print()const
            {
                printf("%03d -> %03d", pFrom->idx, pTo->idx);
                if (item.type == Production::Item::TerminalSymbol)
                {
                    printf("(");
                    item.rule.printShowName();
                    printf(")");
                }
                else printf("(%s)", item.name.c_str());
                printf("\n");
            }

            void print(fstream& fs)const
            {
                fs << string::format("%03d -> %03d", pFrom->idx, pTo->idx);
                if (item.type == Production::Item::TerminalSymbol)
                {
                    fs << "(";
                    item.rule.printShowName(fs);
                    fs << ")";
                }
                else fs << string::format("(%s)", item.name.c_str());
                fs << endl;
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
        void print(const string& path);
    protected:
        void closure(Item* pKernel, vector<Production::Item>& vts);
        void go(const LALR1Production& p, Item* pTo, const Production::Item& item);
        void first(vector<Production::Item>::const_iterator first, vector<Production::Item>::const_iterator last, const vector<LALR1Production::Item>& wildCards, vector<LALR1Production::Item>& v);
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
