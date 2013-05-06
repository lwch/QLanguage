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

#include "LRProduction.h"
#include "BasicParser.h"
#include "../Lexer/Lexer.h"
#include "../../QCore/Library/function_additional.h"

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
#if defined(_DEBUG) && DEBUG_LEVEL == 3
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
                stream << string::format("%03d -> %03d", pFrom->idx, pTo->idx);
                if (item.type == Production::Item::TerminalSymbol)
                {
                    stream << "(";
                    item.rule.printShowName(stream);
                    stream << ")";
                }
                else
                {
#if defined(_DEBUG) && DEBUG_LEVEL == 3
                    stream << string::format("(%s)", item.name.c_str());
#else
                    stream << "(VT)";
#endif
                }
                stream << endl;
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
        LALR1();
        LALR1(const vector<Production>& productions, Production::Item& start);

        void setProductions(const vector<Production>& productions);
        void setStart(Production::Item* start);

        bool make();
        void output(const string& path);
        bool parse(const list<Lexer::Token>& l, BasicParser* pParser);
        vector<Production> rules();

        void print(Library::ostream& stream);
    protected:
        Item* closure(const vector<LALR1Production>& kernel);
        void firstX(const LALR1Production& p, vector<Production::Item>& v, size_t idx);
        void first(const LALR1Production& p, vector<Production::Item>& v, size_t idx);
        void symbols(Item* pItem, vector<Production::Item>& v);
        bool go(Item* pItem, const Production::Item& x, Item*& newItem);
        long itemIndex(Item* pItem);
        bool buildParserTable();
        long index_of_vt(const string& str, long idx = 0);
        long getGoTo(ushort s, const Production::Item& i);
        static inline bool compare_production_item_is_vt(const Production::Item& i);
        static inline bool compare_production_item_is_vn(const Production::Item& i);
        static inline bool compare_edge_item_is(const Edge& e, const Production::Item& x);
    protected:
        Context context;

        Item* pStart;
        vector<Item*> items;
        hashmap<Item*, vector<Edge> > edges;
        vector<pair<uchar, ushort> >  table;

        vector<Production::Item> vts;
        vector<Production::Item> vns;

        vector<LALR1Production> _rules;
        map<Production::Item, vector<LALR1Production> > inputProductions;
        Production::Item begin;
        Production::Item* start;
    };
}

#endif
