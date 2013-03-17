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
        struct Item
        {
            vector<LR0Production> data;
            uint idx;

            Item() { idx = inc(); }

            static uint inc()
            {
                static uint i = 0;
                return i++;
            }
        };

        struct Edge 
        {
            Item* pStart;
            Item* pEnd;
            Rule  rule;

            Edge(Item* pStart, Item* pEnd, const Rule& rule) : pStart(pStart), pEnd(pEnd), rule(rule) {}
        };

        struct Context
        {
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
        LR0();
        LR0(const vector<Production>& productions, const Production::Item& start);

        bool make();
    protected:
        Item* closure(const vector<LR0Production>& x);
    protected:
        Production::Item start;
        map<Production::Item, vector<Production> > productionMap;
        vector<Production> inputProductions;
        hashmap<Item*, vector<Edge> > edges;
        Context context;
    };
}

#endif
