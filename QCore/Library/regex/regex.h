/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:46
	filename: 	\QCore\Library\regex\regex.h
	file path:	\QCore\Library\regex
	file base:	regex
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_REGEX_REGEX_H_
#define _QLANGUAGE_LIBRARY_REGEX_REGEX_H_

#include "../definition.h"
#include "../typedef.h"

#include "../vector.h"
#include "../set.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace regex
{
    template <typename Char_Type, typename String_Type>
    class Rule
    {
    public:
        struct State;
    protected:
        struct Edge
        {
            struct
            {
                Char_Type   char_value;
                String_Type string_value;
            }data;

            enum { TEpsilon, TChar, TString } edge_type;

            State* pFrom;
            State* pTo;

            Edge(State* pFrom, State* pTo) : edge_type(TEpsilon), pFrom(pFrom), pTo(pTo) {}

            Edge(const Char_Type& x, State* pFrom, State* pTo) : edge_type(TChar), pFrom(pFrom), pTo(pTo)
            {
                data.char_value = x;
            }

            Edge(const String_Type& x, State* pFrom, State* pTo) : edge_type(TString), pFrom(pFrom), pTo(pTo)
            {
                data.string_value = x;
            }
        };
    public:
        struct State
        {
#ifdef _DEBUG
            UINT idx;

            State()
            {
                idx = inc();
            }

            static UINT inc()
            {
                static UINT i = 0;
                return i++;
            }
#else
            State() {}
#endif
        };

        struct Context
        {
            set<State*> states;

            ~Context()
            {
                for (typename set<State*>::iterator i = states.begin(), m = states.end(); i != m; ++i)
                {
                    State_Alloc::deallocate(*i);
                }
                states.clear();
            }
        };
    protected:
        typedef Rule<Char_Type, String_Type> self;
        typedef allocator<State> State_Alloc;
    public:
        Rule(const Char_Type& x, Context& context) : context(context)
        {
            pStart = State_Alloc::allocate();
            construct(pStart);

            pEnd = State_Alloc::allocate();
            construct(pEnd);

            edges.push_back(Edge(x, pStart, pEnd));

            context.states.insert(pStart);
            context.states.insert(pEnd);
        }

        Rule(const self& x) : context(x.context)
        {
            pStart = x.pStart;
            pEnd = x.pEnd;
            edges = x.edges;
        }

        ~Rule() {}

        self operator+(const self& x)
        {
            self a = *this;
            for (int i = 0, m = x.edges.size(); i < m; ++i)
            {
                a.edges.push_back(x.edges[i]);
            }
            a.edges.push_back(Edge(a.pEnd, x.pStart));
            pEnd = x.pEnd;
            return a;
        }

        inline self& operator=(const self& x)
        {
            if (&x != this)
            {
                pStart = x.pStart;
                pEnd = x.pEnd;
                edges = x.edges;
            }
            return *this;
        }

#ifdef _DEBUG
        void print()
        {
            for (typename vector<Edge>::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
            {
                printf("%03d -> %03d", i->pFrom->idx, i->pTo->idx);
                switch (i->edge_type)
                {
                case Edge::TEpsilon:
                    printf("(¦Å)\n");
                    break;
                case Edge::TChar:
                    printf("(%c)\n", i->data.char_value);
                    break;
                case Edge::TString:
                    printf("(%s)\n", i->data.string_value.c_str());
                    break;
                }
            }
            printf("start: %03d -> end: %03d\n", pStart->idx, pEnd->idx);
        }
#endif
    protected:
        State *pStart, *pEnd;
        vector<Edge> edges;
        Context&     context;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
