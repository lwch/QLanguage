﻿/********************************************************************
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

#include "LRProduction.h"

namespace QLanguage
{
    class LALR1
    {
    public:
        class Item
        {
        public:
            vector<LALR1Production> data;
            uint idx;

            Item() : idx(0) {}

            void mergeWildCards(Item* pItem)
            {
#ifdef _DEBUG
                if (data.size() != pItem->data.size()) throw error<const char*>("compare size error", __FILE__, __LINE__);
#endif
                for (size_t i = 0, m = data.size(); i < m; ++i)
                {
                    data[i].wildCards.add_unique(pItem->data[i].wildCards);
                }
            }

            inline const bool operator==(const Item& x)const
            {
                return data == x.data;
            }

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

            void print(Library::ostream& stream)const
            {
#ifdef _DEBUG
                stream << string::format("%03d -> %03d", pFrom->idx, pTo->idx);
                if (item.type == Production::Item::TerminalSymbol)
                {
                    stream << "(";
                    item.rule.printShowName(stream);
                    stream << ")";
                }
                else stream << string::format("(%s)", item.name.c_str());
                stream << endl;
#endif
            }
        };
    protected:
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
    protected:
        Item* closure(const vector<LALR1Production>& kernel);
        void firstX(const LALR1Production& p, vector<Production::Item>& v, size_t idx);
        void first(const LALR1Production& p, vector<Production::Item>& v, size_t idx);
        void symbols(Item* pItem, vector<Production::Item>& v);
        bool go(Item* pItem, const Production::Item& x, Item*& newItem);
        void fromItoJ(const LALR1Production& p, vector<LALR1Production>& v);
        long itemIndex(Item* pItem);
    protected:
        Context context;

        Item* pStart;
        vector<Item*> items;
        hashmap<Item*, vector<Edge> > edges;

        map<Production::Item, vector<LALR1Production> > inputProductions;
        Production::Item begin;
        const Production::Item& start;
    };
}

#endif
