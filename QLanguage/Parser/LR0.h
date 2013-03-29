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

#include "LRProduction.h"

using namespace QLanguage::Library;

namespace QLanguage
{
    class LR0
    {
        class Item
        {
        public:
            vector<LR0Production> data;
            uint idx;

            Item() { idx = inc(); }

            static uint inc()
            {
                static uint i = 0;
                return i++;
            }

            inline const bool operator==(const Item& x)const
            {
                return data == x.data;
            }
        };

        struct Edge 
        {
            Item* pStart;
            Item* pEnd;
            Production::Item item;

            Edge(Item* pStart, Item* pEnd, const Production::Item& item) : pStart(pStart), pEnd(pEnd), item(item) {}
        };

        class Context
        {
        public:
            set<Item*> states;

            bool isItemExists(Item* pItem)
            {
                for (set<Item*>::const_iterator i = states.begin(), m = states.end(); i != m; ++i)
                {
                    if (*(*i) == *pItem) return true;
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
        LR0();
        LR0(const vector<Production>& productions, const Production::Item& start);

        bool make();

        void print();
    protected:
        Item* closure(const vector<LR0Production>& x);
        void closure(const LR0Production& x, vector<LR0Production>& y);
        Item* go(Item* i, const Production::Item& x);
        void vs(Item* i, vector<Production::Item>& v);
    protected:
        Production::Item start;
        map<Production::Item, vector<Production> > productionMap;
        vector<Production> inputProductions;

        hashmap<Item*, vector<Edge> > edges;
        Item* pStart;

        Context context;
    };
}

#endif
