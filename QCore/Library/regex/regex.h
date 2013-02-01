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
#include "../map.h"

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

            const bool operator<(const Edge& x)const
            {
                return pFrom < x.pFrom;
            }

            const bool operator==(const Edge& x)const
            {
                return pFrom == x.pFrom && pTo == x.pTo;
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
        Rule(Context& context) : context(context), pStart(NULL), pEnd(NULL) {}

        Rule(const Char_Type& x, Context& context) : context(context)
        {
            pStart = State_Alloc::allocate();
            construct(pStart);

            pEnd = State_Alloc::allocate();
            construct(pEnd);

            edges.insert(Edge(x, pStart, pEnd));

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
            self a = clone(*this), b = clone(x);
            copyEdges(b, a);
            a.edges.insert(Edge(a.pEnd, b.pStart));
            a.pEnd = b.pEnd;
            return a;
        }

        self operator-(const self& x)
        {
            self a = clone(*this);

            if (edges.size() == 1 && x.edges.size() == 1 &&
                edges.begin()->edge_type == Edge::TChar && x.edges.begin()->edge_type == Edge::TChar)
            {
                State* _pStart = State_Alloc::allocate();
                construct(_pStart);

                State* _pEnd   = State_Alloc::allocate();
                construct(_pEnd);

                context.states.insert(_pStart);
                context.states.insert(_pEnd);

                a.edges.insert(Edge(_pStart, a.pStart));
                a.edges.insert(Edge(a.pEnd, _pEnd));

                const Char_Type chStart = edges.begin()->data.char_value;
                const Char_Type chEnd   = x.edges.begin()->data.char_value;
                for (Char_Type ch = chStart + 1; ch < chEnd; ++ch)
                {
                    self y(ch, context);
                    copyEdges(y, a);
                    a.edges.insert(Edge(_pStart, y.pStart));
                    a.edges.insert(Edge(y.pEnd, _pEnd));
                }

                self b = clone(x);
                copyEdges(b, a);

                a.edges.insert(Edge(_pStart, b.pStart));
                a.edges.insert(Edge(b.pEnd, _pEnd));

                a.pStart = _pStart;
                a.pEnd   = _pEnd;
            }
            else
            {
                throw error<char*>("doesn't support", __FILE__, __LINE__);
            }
            return a;
        }

        self operator|(const self& x)
        {
            self a = clone(*this), b = clone(x);
            copyEdges(b, a);

            State* _pStart = State_Alloc::allocate();
            construct(_pStart);

            State* _pEnd = State_Alloc::allocate();
            construct(_pEnd);

            context.states.insert(_pStart);
            context.states.insert(_pEnd);

            a.edges.insert(Edge(_pStart, a.pStart));
            a.edges.insert(Edge(_pStart, b.pStart));
            a.edges.insert(Edge(a.pEnd, _pEnd));
            a.edges.insert(Edge(x.pEnd, _pEnd));

            a.pStart = _pStart;
            a.pEnd   = _pEnd;

            return a;
        }

        self operator*()
        {
            self a = clone(*this);

            State* pState = State_Alloc::allocate();
            construct(pState);

            context.states.insert(pState);

            a.edges.insert(Edge(pState, a.pStart));
            a.edges.insert(Edge(a.pEnd, pState));

            a.pStart = a.pEnd = pState;

            return a;
        }

        self operator+()
        {
            self a = clone(*this), b = clone(*this);
            copyEdges(b, a);

            State* _pEnd   = State_Alloc::allocate();
            construct(_pEnd);

            context.states.insert(_pEnd);

            a.edges.insert(Edge(a.pEnd, _pEnd));
            a.edges.insert(Edge(_pEnd, b.pStart));
            a.edges.insert(Edge(b.pEnd, _pEnd));

            a.pEnd = _pEnd;

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
            for (typename set<Edge>::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
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
        static void copyEdges(const self& from, self& to)
        {
            for (set<Edge>::const_iterator i = from.edges.begin(), m = from.edges.end(); i != m; ++i)
            {
                to.edges.insert(*i);
            }
        }

        static self clone(const self& x)
        {
            self result(x.context);

            map<State*, State*> statesMap;
            for (set<Edge>::const_iterator i = x.edges.begin(), m = x.edges.end(); i != m; ++i)
            {
                State *pFrom = statesMap[i->pFrom], *pTo = statesMap[i->pTo];
                if (pFrom == NULL)
                {
                    pFrom = statesMap[i->pFrom] = State_Alloc::allocate();
                    construct(pFrom);

                    x.context.states.insert(pFrom);
                }

                if (pTo == NULL)
                {
                    pTo = statesMap[i->pTo] = State_Alloc::allocate();
                    construct(pTo);

                    x.context.states.insert(pTo);
                }

                Edge edge(*i);
                edge.pFrom = pFrom;
                edge.pTo   = pTo;
                result.edges.insert(edge);
            }

            result.pStart = statesMap[x.pStart];
            result.pEnd   = statesMap[x.pEnd];

            return result;
        }
    protected:
        State *pStart, *pEnd;
        set<Edge> edges;
        Context&  context;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
