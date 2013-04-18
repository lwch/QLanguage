/********************************************************************
	created:	2013/03/09
	created:	9:3:2013   14:26
	filename: 	\QLanguage\Parser\LR0.h
	file path:	\QLanguage\Parser
	file base:	LR0
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LR0_H_
#define _QLANGUAGE_LR0_H_

#include "../../QCore/Library/vector.h"
#include "../../QCore/Library/hashmap.h"
#include "../../QCore/Library/hashset.h"

#include "LRProduction.h"

using namespace QLanguage::Library;

namespace QLanguage
{
    class LR0
    {
        friend class LALR1;

        class Item
        {
        public:
            map<Production::Item, vector<LR0Production> > data;
#ifdef _DEBUG
            uint idx;

            Item() : idx(inc()) {}

            static uint inc()
            {
                static uint i = 0;
                return i++;
            }
#else
            Item () {}
#endif

            inline const bool operator==(const Item& x)const
            {
                return data == x.data;
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

            bool isItemExists(Item* pItem, Item*& pOldItem)
            {
                for (set<Item*>::const_iterator i = states.begin(), m = states.end(); i != m; ++i)
                {
                    if (*(*i) == *pItem)
                    {
                        pOldItem = *i;
                        return true;
                    }
                }
                return false;
            }

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
        LR0(const vector<Production>& productions, const Production::Item& start);

        bool make();

        void print();
        void print(const string& path);
    protected:
        Item* closure(const vector<LR0Production>& x);
        void closure(const LR0Production& x, map<LR0Production::Item, vector<LR0Production> >& y);
        pair<Item*, bool> go(Item* i, const Production::Item& x);
        void vs(Item* i, vector<Production::Item>& v);
    protected:
        Production::Item begin;
        Production::Item start;
        map<Production::Item, vector<Production> > productionMap;
        vector<Production> inputProductions;
        vector<Item*>      items;

        hashmap<Item*, vector<Edge> > edges;
        Item*      pStart;
        set<Item*> pEnds;

        Context context;
    };
}

#endif
