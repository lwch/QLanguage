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
#else
            Item() {}
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
        LALR1(const vector<Production>& productions, const Production::Item& start);

        bool make();

        void print();
        void print(const string& path);
    protected:
        void closure(vector<LALR1Production>& kernel);
        void afirst(const LALR1Production& p, size_t pos, vector<LALR1Production::Item>& v);
        void first(const LALR1Production& p, size_t pos, vector<LALR1Production::Item>& v);
//         void closure(Item* pItem, bool& bContinue);
//         Item* go(Item* pItem, const Production::Item& x);
//         void afirst(const LALR1Production& p, vector<LALR1Production::Item>& v);
//         void appendWildCards(Item* pItem, const Production::Item& left, const vector<LALR1Production::Item>& v, vector<Production::Item>& vts, bool& bContinue);
//         void spreadWildCards(Item* pItem, const vector<LALR1Production::Item>& v, bool& bContinue);
    protected:
        map<Production::Item, map<size_t, vector<LALR1Production> > > productionMap;
        Production::Item begin;

        Item*      pStart;
        set<Item*> pEnd;
        hashmap<Item*, vector<Edge> > edges;
        vector<Item*> items;

        Context context;
    };
}

#endif
