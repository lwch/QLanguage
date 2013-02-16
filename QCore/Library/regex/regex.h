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

#include "../set.h"
#include "../map.h"
#include "../hashmap.h"
#include "../queue.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace regex
{
    template <typename Char_Type, typename String_Type>
    class Rule
    {
    protected:
        struct EpsilonNFA_State
        {
#ifdef _DEBUG
            uint idx;

            EpsilonNFA_State()
            {
                idx = inc();
            }

            inline uint inc()
            {
                static uint i = 0;
                return i++;
            }
#else
            EpsilonNFA_State() {}
#endif
        };

        struct EpsilonNFA_Edge
        {
            struct
            {
                Char_Type   char_value;
                String_Type string_value;
            }data;

            enum Edge_Type
            {
                TUnknown = 0,
                TNot     = 1,
                TChar    = 2,
                TString  = 4,
                TEpsilon = 8
            };
            
            uchar edge_type;

            EpsilonNFA_State* pFrom;
            EpsilonNFA_State* pTo;

            EpsilonNFA_Edge(EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : edge_type(TEpsilon), pFrom(pFrom), pTo(pTo) {}

            EpsilonNFA_Edge(const Char_Type& x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : edge_type(TChar), pFrom(pFrom), pTo(pTo)
            {
                data.char_value = x;
            }

            EpsilonNFA_Edge(const String_Type& x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : edge_type(TString), pFrom(pFrom), pTo(pTo)
            {
                data.string_value = x;
            }

            inline void negate()
            {
                edge_type ^= TNot;
            }

            inline const bool isNot()const
            {
                return (edge_type & TNot) == TNot;
            }

            inline const bool isEpsilon()const
            {
                return (edge_type & TEpsilon) == TEpsilon;
            }

            inline const bool isChar()const
            {
                return (edge_type & TChar) == TChar;
            }

            inline const bool isString()const
            {
                return (edge_type & TString) == TString;
            }

            const Edge_Type edgeType()const
            {
                if (isEpsilon()) return TEpsilon;
                else if (isChar()) return TChar;
                else if (isString()) return TString;
                else return TUnknown;
            }

            const bool operator<(const EpsilonNFA_Edge& x)const
            {
                return (ulong)pFrom + pTo < (ulong)x.pFrom + x.pTo;
            }

            const bool operator==(const EpsilonNFA_Edge& x)const
            {
                return pFrom == x.pFrom && pTo == x.pTo;
            }
        };

        class _hash
        {
        public:
            const HASH_KEY_TYPE operator()(const EpsilonNFA_State* x)const
            {
                return (HASH_KEY_TYPE)x;
            }
        };

        struct DFA_State
        {
            set<EpsilonNFA_State*> content;
            bool                   bFlag;
#ifdef _DEBUG
            uint                   idx;
#endif

            DFA_State(const set<EpsilonNFA_State*>& x) : content(x), bFlag(false)
            {
#ifdef _DEBUG
                idx = inc();
#endif
            }

            inline const bool operator==(const DFA_State& x)const
            {
                if (&x == this) return true;

                return content == x.content;
            }

#ifdef _DEBUG
            inline uint inc()
            {
                static uint i = 0;
                return i++;
            }
#endif
        };

        struct DFA_Edge
        {
            struct
            {
                Char_Type   char_value;
                String_Type string_value;
            }data;

            enum Edge_Type
            {
                TUnknown = 0,
                TNot     = 1,
                TChar    = 2,
                TString  = 4
            };

            uchar edge_type;

            DFA_State* pFrom;
            DFA_State* pTo;

            DFA_Edge(const Char_Type& x, bool bNot, DFA_State* pFrom, DFA_State* pTo) : pFrom(pFrom), pTo(pTo)
            {
                data.char_value = x;
                edge_type = bNot ? (TChar | TNot) : TChar;
            }

            DFA_Edge(const String_Type& x, bool bNot, DFA_State* pFrom, DFA_State* pTo) : pFrom(pFrom), pTo(pTo)
            {
                data.string_value = x;
                edge_type = bNot ? (TString | TNot) : TString;
            }

            inline const bool isNot()const
            {
                return (edge_type & TNot) == TNot;
            }

            inline const bool isChar()const
            {
                return (edge_type & TChar) == TChar;
            }

            inline const bool isString()const
            {
                return (edge_type & TString) == TString;
            }

            const Edge_Type edgeType()const
            {
                if (isChar()) return TChar;
                else if (isString()) return TString;
                else return TUnknown;
            }

            const bool operator<(const DFA_Edge& x)const
            {
                return (ulong)pFrom + pTo < (ulong)x.pFrom + x.pTo;
            }

            const bool operator==(const DFA_Edge& x)const
            {
                return pFrom == x.pFrom && pTo == x.pTo;
            }
        };

        struct EpsilonClosureInfo
        {
            set<EpsilonNFA_State*>        states;
            set<pair<Char_Type, bool> >   chars;
            set<pair<String_Type, bool> > strings;

            EpsilonClosureInfo() {}

            EpsilonClosureInfo(const set<EpsilonNFA_State*>& states,
                               const set<pair<Char_Type, bool> >& chars,
                               const set<pair<String_Type, bool> >& strings)
                               : states(states)
                               , chars(chars)
                               , strings(strings) {}

            EpsilonClosureInfo(const EpsilonClosureInfo& x)
            {
                states  = x.states;
                chars   = x.chars;
                strings = x.strings;
            }

            EpsilonClosureInfo& operator=(const EpsilonClosureInfo& x)
            {
                if (&x != this)
                {
                    states  = x.states;
                    chars   = x.chars;
                    strings = x.strings;
                }
                return *this;
            }
        };
public:
        struct Context
        {
            set<EpsilonNFA_State*> epsilonNFA_States;
            set<DFA_State*>        dfa_States;

            void clear()
            {
                for (typename set<EpsilonNFA_State*>::iterator i = epsilonNFA_States.begin(), m = epsilonNFA_States.end(); i != m; ++i)
                {
                    destruct(*i, has_destruct(*(*i)));
                    EpsilonNFA_State_Alloc::deallocate(*i);
                }
                epsilonNFA_States.clear();

                for (typename set<DFA_State*>::iterator i = dfa_States.begin(), m = dfa_States.end(); i != m; ++i)
                {
                    destruct(*i, has_destruct(*(*i)));
                    DFA_State_Alloc::deallocate(*i);
                }
                dfa_States.clear();
            }

            ~Context()
            {
                clear();
            }
        };
    protected:
        typedef Rule<Char_Type, String_Type> self;
        typedef allocator<EpsilonNFA_State>  EpsilonNFA_State_Alloc;
        typedef allocator<DFA_State>         DFA_State_Alloc;
    public:
        Rule(Context& context) : pEpsilonStart(NULL), pEpsilonEnd(NULL), pDFAStart(NULL), context(context) {}

        Rule(const Char_Type& x, Context& context) : pDFAStart(NULL), context(context)
        {
            pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonStart);

            pEpsilonEnd   = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonEnd);

            epsilonNFA_Edges[pEpsilonStart].push_back(EpsilonNFA_Edge(x, pEpsilonStart, pEpsilonEnd));

            context.epsilonNFA_States.insert(pEpsilonStart);
            context.epsilonNFA_States.insert(pEpsilonEnd);
        }

        Rule(const String_Type& x, Context& context) : pDFAStart(NULL), context(context)
        {
            pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonStart);

            pEpsilonEnd   = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonEnd);

            epsilonNFA_Edges[pEpsilonStart].push_back(EpsilonNFA_Edge(x, pEpsilonStart, pEpsilonEnd));

            context.epsilonNFA_States.insert(pEpsilonStart);
            context.epsilonNFA_States.insert(pEpsilonEnd);
        }

        Rule(const self& x) : context(x.context)
        {
            pEpsilonStart    = x.pEpsilonStart;
            pEpsilonEnd      = x.pEpsilonEnd;
            epsilonNFA_Edges = x.epsilonNFA_Edges;

            pDFAStart = x.pDFAStart;
            pDFAEnds  = x.pDFAEnds;
            dfa_Edges = x.dfa_Edges;
        }

        ~Rule() {}

        self operator+(const self& x)
        {
            self a(context), b(x.context);
            cloneEpsilonNFA(*this, a);
            cloneEpsilonNFA(x, b);
            copyEpsilonNFA_Edges(b, a);

            a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, b.pEpsilonStart));
            a.pEpsilonEnd = b.pEpsilonEnd;
            return a;
        }

        self operator-(const self& x)
        {
            self a(context);
            cloneEpsilonNFA(*this, a);

            if (epsilonNFA_Edges.size() == 1 && x.epsilonNFA_Edges.size() == 1 &&
                epsilonNFA_Edges.begin()->second.size() == 1 && x.epsilonNFA_Edges.begin()->second.size() == 1 &&
                epsilonNFA_Edges.begin()->second.begin()->edge_type == EpsilonNFA_Edge::TChar && x.epsilonNFA_Edges.begin()->second.begin()->edge_type == EpsilonNFA_Edge::TChar)
            {
                EpsilonNFA_State* _pStart = EpsilonNFA_State_Alloc::allocate();
                construct(_pStart);

                EpsilonNFA_State* _pEnd   = EpsilonNFA_State_Alloc::allocate();
                construct(_pEnd);

                context.epsilonNFA_States.insert(_pStart);
                context.epsilonNFA_States.insert(_pEnd);

                a.epsilonNFA_Edges[_pStart].push_back(EpsilonNFA_Edge(_pStart, a.pEpsilonStart));
                a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, _pEnd));

                const Char_Type chStart = epsilonNFA_Edges.begin()->second.begin()->data.char_value;
                const Char_Type chEnd   = x.epsilonNFA_Edges.begin()->second.begin()->data.char_value;
                for (Char_Type ch = chStart + 1; ch < chEnd; ++ch)
                {
                    self y(ch, context);
                    copyEpsilonNFA_Edges(y, a);
                    a.epsilonNFA_Edges[_pStart].push_back(EpsilonNFA_Edge(_pStart, y.pEpsilonStart));
                    a.epsilonNFA_Edges[y.pEpsilonEnd].push_back(EpsilonNFA_Edge(y.pEpsilonEnd, _pEnd));
                }

                self b(x.context);
                cloneEpsilonNFA(x, b);
                copyEpsilonNFA_Edges(b, a);

                a.epsilonNFA_Edges[_pStart].push_back(EpsilonNFA_Edge(_pStart, b.pEpsilonStart));
                a.epsilonNFA_Edges[b.pEpsilonEnd].push_back(EpsilonNFA_Edge(b.pEpsilonEnd, _pEnd));

                a.pEpsilonStart = _pStart;
                a.pEpsilonEnd   = _pEnd;
            }
            else
            {
                throw error<string>("doesn't support", __FILE__, __LINE__);
            }
            return a;
        }

        self operator|(const self& x)
        {
            self a(context), b(x.context);
            cloneEpsilonNFA(*this, a);
            cloneEpsilonNFA(x, b);
            copyEpsilonNFA_Edges(b, a);

            EpsilonNFA_State* _pStart = EpsilonNFA_State_Alloc::allocate();
            construct(_pStart);

            EpsilonNFA_State* _pEnd = EpsilonNFA_State_Alloc::allocate();
            construct(_pEnd);

            context.epsilonNFA_States.insert(_pStart);
            context.epsilonNFA_States.insert(_pEnd);

            a.epsilonNFA_Edges[_pStart].push_back(EpsilonNFA_Edge(_pStart, a.pEpsilonStart));
            a.epsilonNFA_Edges[_pStart].push_back(EpsilonNFA_Edge(_pStart, b.pEpsilonStart));
            a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, _pEnd));
            a.epsilonNFA_Edges[b.pEpsilonEnd].push_back(EpsilonNFA_Edge(b.pEpsilonEnd, _pEnd));

            a.pEpsilonStart = _pStart;
            a.pEpsilonEnd   = _pEnd;

            return a;
        }

        inline self opt()
        {
            self a(context);
            cloneEpsilonNFA(*this, a);
            a.epsilonNFA_Edges[a.pEpsilonStart].push_back(EpsilonNFA_Edge(a.pEpsilonStart, a.pEpsilonEnd));
            return a;
        }

        self operator*()
        {
            self a(context);
            cloneEpsilonNFA(*this, a);

            a.epsilonNFA_Edges[a.pEpsilonStart].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, a.pEpsilonStart));

            a.pEpsilonEnd = a.pEpsilonStart;

            return a;
        }

        self operator+()
        {
            self a(context);
            cloneEpsilonNFA(*this, a);
            
            a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, a.pEpsilonStart));

            return a;
        }

        self operator!()
        {
            self a = cloneEpsilonNFA(*this);
            for (typename hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hash>::iterator i = a.epsilonNFA_Edges.begin(), m = a.epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (typename vector<EpsilonNFA_Edge>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    if (j->isChar() || j->isString()) j->negate();
                }
            }
            return a;
        }

        inline self& operator=(const self& x)
        {
            if (&x != this)
            {
                pEpsilonStart    = x.pEpsilonStart;
                pEpsilonEnd      = x.pEpsilonEnd;
                epsilonNFA_Edges = x.epsilonNFA_Edges;

                pDFAStart = x.pDFAStart;
                pDFAEnds  = x.pDFAEnds;
                dfa_Edges = x.dfa_Edges;
            }
            return *this;
        }

        void buildDFA()
        {
            set<EpsilonNFA_State*> start;
            start.insert(pEpsilonStart);

            typedef pair<DFA_State*, EpsilonClosureInfo> c_type;

            map<size_t, list<c_type> > c;
            queue<c_type> c2;

            pDFAStart = DFA_State_Alloc::allocate();
            EpsilonClosureInfo info;
            epsilonClosure(start, info);
            construct(pDFAStart, info.states);

            c_type ct(pDFAStart, info);
            c[info.states.size()].push_back(ct);
            c2.push(ct);

            if (isEndDFAStatus(pDFAStart)) pDFAEnds.insert(pDFAStart);
            context.dfa_States.insert(pDFAStart);

            while (!c2.empty())
            {
                DFA_State* t = c2.front().first;
                set<pair<Char_Type, bool> > chars = c2.front().second.chars;
                set<pair<String_Type, bool> > strings = c2.front().second.strings;
                t->bFlag = true;

                for (typename set<pair<Char_Type, bool> >::const_iterator i = chars.begin(), m = chars.end(); i != m; ++i)
                {
                    EpsilonClosureInfo info;
                    epsilonClosure(move(*t, i->first, i->second), info);

                    DFA_State* pState = DFA_State_Alloc::allocate();
                    construct(pState, info.states);

                    DFA_State* p = getDFAState(pState, c);
                    if (p) // 如果这个状态已存在
                    {
                        destruct(pState, has_destruct(*pState));
                        DFA_State_Alloc::deallocate(pState);

                        dfa_Edges.insert(DFA_Edge(i->first, i->second, t, p));
                    }
                    else
                    {
                        if (isEndDFAStatus(pState)) pDFAEnds.insert(pState);

                        c_type ct(pState, info);
                        c[info.states.size()].push_back(ct);
                        c2.push(ct);

                        dfa_Edges.insert(DFA_Edge(i->first, i->second, t, pState));
                    }
                }

                for (typename set<pair<String_Type, bool> >::const_iterator i = strings.begin(), m = strings.end(); i != m; ++i)
                {
                    EpsilonClosureInfo info;
                    epsilonClosure(move(*t, i->first, i->second), info);

                    DFA_State* pState = DFA_State_Alloc::allocate();
                    construct(pState, info.states);

                    DFA_State* p = getDFAState(pState, c);
                    if (p) // 如果这个状态已存在
                    {
                        destruct(pState, has_destruct(*pState));
                        DFA_State_Alloc::deallocate(pState);

                        dfa_Edges.insert(DFA_Edge(i->first, i->second, t, p));
                    }
                    else
                    {
                        if (isEndDFAStatus(pState)) pDFAEnds.insert(pState);

                        c_type ct(pState, info);
                        c[info.states.size()].push_back(ct);
                        c2.push(ct);

                        dfa_Edges.insert(DFA_Edge(i->first, i->second, t, pState));
                    }
                }
                c2.pop();
            }
        }

        void clear()
        {
            pEpsilonStart = pEpsilonEnd = NULL;
            epsilonNFA_Edges.clear();

            pDFAStart = NULL;
            pDFAEnds.clear();
            dfa_Edges.clear();

            context.clear();
        }

#ifdef _DEBUG
        void printEpsilonNFA()
        {
            printf("-------- ε- NFA Start --------\n");
            for (typename hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hash>::const_iterator i = epsilonNFA_Edges.begin(), m = epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (typename vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    printf("%03d -> %03d", j->pFrom->idx, j->pTo->idx);
                    switch (j->edgeType())
                    {
                    case EpsilonNFA_Edge::TEpsilon:
                        printf("(ε)");
                        break;
                    case EpsilonNFA_Edge::TChar:
                        printf("(%c)", j->data.char_value);
                        break;
                    case EpsilonNFA_Edge::TString:
                        printf("(%s)", j->data.string_value.c_str());
                        break;
                    default:
                        break;
                    }
                    if (j->isNot()) printf("(not)");
                    printf("\n");
                }
            }
            printf("start: %03d -> end: %03d\n", pEpsilonStart->idx, pEpsilonEnd->idx);
            printf("--------- ε- NFA End ---------\n");
        }

        void printDFA()
        {
            printf("---------- DFA Start ----------\n");
            set<DFA_State*> tmp;
            for (typename set<DFA_Edge>::const_iterator i = dfa_Edges.begin(), m = dfa_Edges.end(); i != m; ++i)
            {
                printf("%03d -> %03d", i->pFrom->idx, i->pTo->idx);
                switch (i->edgeType())
                {
                case DFA_Edge::TChar:
                    printf("(%c)", i->data.char_value);
                    break;
                case DFA_Edge::TString:
                    printf("(%s)", i->data.string_value.c_str());
                    break;
                default:
                    break;
                }
                if (i->isNot()) printf("(not)");
                printf("\n");
                tmp.insert(i->pFrom);
                tmp.insert(i->pTo);
            }

            printf("start: %03d -> ends: ", pDFAStart->idx);
            for (typename set<DFA_State*>::const_iterator i = pDFAEnds.begin(), m = pDFAEnds.end(); i != m; ++i)
            {
                printf("%03d ", (*i)->idx);
            }
            printf("\n");
#if DEBUG_LEVEL == 3
            printf("-------------------------------\n");

            for (typename set<DFA_State*>::const_iterator i = tmp.begin(), m = tmp.end(); i != m; ++i)
            {
                printf("State: %03d\n", (*i)->idx);
                for (typename set<EpsilonNFA_State*>::const_iterator j = (*i)->content.begin(), n = (*i)->content.end(); j != n; ++j)
                {
                    printf("%03d ", (*j)->idx);
                }
                printf("\n");
            }
#endif
            printf("----------- DFA End -----------\n");
        }
#endif
    protected:
        static void copyEpsilonNFA_Edges(const self& from, self& to)
        {
            for (typename hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hash>::const_iterator i = from.epsilonNFA_Edges.begin(), m = from.epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (typename vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    to.epsilonNFA_Edges[i->first].push_back(*j);
                }
            }
        }

        static void cloneEpsilonNFA(const self& x, self& to)
        {
            map<EpsilonNFA_State*, EpsilonNFA_State*> statesMap;
            for (typename hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hash>::const_iterator i = x.epsilonNFA_Edges.begin(), m = x.epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (typename vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    EpsilonNFA_State *pFrom = statesMap[j->pFrom], *pTo = statesMap[j->pTo];
                    if (pFrom == NULL)
                    {
                        pFrom = statesMap[j->pFrom] = EpsilonNFA_State_Alloc::allocate();
                        construct(pFrom);

                        to.context.epsilonNFA_States.insert(pFrom);
                    }

                    if (pTo == NULL)
                    {
                        pTo = statesMap[j->pTo] = EpsilonNFA_State_Alloc::allocate();
                        construct(pTo);

                        to.context.epsilonNFA_States.insert(pTo);
                    }

                    EpsilonNFA_Edge edge(*j);
                    edge.pFrom = pFrom;
                    edge.pTo   = pTo;
                    to.epsilonNFA_Edges[pFrom].push_back(edge);
                }
            }

            to.pEpsilonStart = statesMap[x.pEpsilonStart];
            to.pEpsilonEnd   = statesMap[x.pEpsilonEnd];
        }

        void epsilonClosure(const set<EpsilonNFA_State*>& k, EpsilonClosureInfo& info)
        {
            for (typename set<EpsilonNFA_State*>::const_iterator i = k.begin(), m = k.end(); i != m; ++i)
            {
                info.states.insert(*i);
                set<EpsilonNFA_State*> tmp;
                tmp.insert(*i);
                while (!tmp.empty())
                {
                    EpsilonNFA_State* pState = *tmp.begin();
                    for (typename vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[pState].begin(), n = epsilonNFA_Edges[pState].end(); j != n; ++j)
                    {
                        if (j->isEpsilon())
                        {
                            if (info.states.insert(j->pTo).second) tmp.insert(j->pTo);
                        }
                        else if (j->isChar()) info.chars.insert(pair<Char_Type, bool>(j->data.char_value, j->isNot()));
                        else if (j->isString()) info.strings.insert(pair<String_Type, bool>(j->data.string_value, j->isNot()));
                    }
                    tmp.erase(pState);
                }
            }
        }

        set<EpsilonNFA_State*> move(const DFA_State& t, const Char_Type& c, bool bNot)
        {
            set<EpsilonNFA_State*> result;
            for (typename set<EpsilonNFA_State*>::const_iterator i = t.content.begin(), m = t.content.end(); i != m; ++i)
            {
                for (typename vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[*i].begin(), n = epsilonNFA_Edges[*i].end(); j != n; ++j)
                {
                    if (j->isChar() && j->data.char_value == c && j->isNot() == bNot) result.insert(j->pTo);
                }
            }
            return result;
        }

        set<EpsilonNFA_State*> move(const DFA_State& t, const String_Type& s, bool bNot)
        {
            set<EpsilonNFA_State*> result;
            for (typename set<EpsilonNFA_State*>::const_iterator i = t.content.begin(), m = t.content.end(); i != m; ++i)
            {
                for (typename vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[*i].begin(), n = epsilonNFA_Edges[*i].end(); j != n; ++j)
                {
                    if (j->isString() && j->data.string_value == s && j->isNot() == bNot) result.insert(j->pTo);
                }
            }
            return result;
        }

        DFA_State* getDFAState(DFA_State* pNewState, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c)
        {
            for (typename list<pair<DFA_State*, EpsilonClosureInfo> >::const_iterator i = c[pNewState->content.size()].begin(), m = c[pNewState->content.size()].end(); i != m; ++i)
            {
                if (*i->first == *pNewState) return i->first;
            }
            context.dfa_States.insert(pNewState);
            return NULL;
        }

        const bool isEndDFAStatus(const DFA_State* pState)const
        {
            for (typename set<EpsilonNFA_State*>::const_iterator i = pState->content.begin(), m = pState->content.end(); i != m; ++i)
            {
                if (*i == pEpsilonEnd) return true;
            }
            return false;
        }
    protected:
        EpsilonNFA_State *pEpsilonStart, *pEpsilonEnd;
        hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hash> epsilonNFA_Edges;

        DFA_State*      pDFAStart;
        set<DFA_State*> pDFAEnds;
        set<DFA_Edge>   dfa_Edges;

        Context& context;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
