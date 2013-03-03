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
#include "../stack.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace regex
{
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
            union
            {
                struct
                {
                    char value1;
                    char value2;
                }Char;
                struct
                {
                    const char* value;
                }String;
            }data;

            enum Edge_Type
            {
                TUnknown = 0,
                TNot     = 1,
                TFromTo  = 2,
                TChar    = 4,
                TString  = 8,
                TEpsilon = 12
            };
            
            uchar edge_type;

            EpsilonNFA_State* pFrom;
            EpsilonNFA_State* pTo;

            EpsilonNFA_Edge(EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : edge_type(TEpsilon), pFrom(pFrom), pTo(pTo) {}

            EpsilonNFA_Edge(char x, char y, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : edge_type(TFromTo), pFrom(pFrom), pTo(pTo)
            {
                data.Char.value1 = x;
                data.Char.value2 = y;
            }

            EpsilonNFA_Edge(char x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : edge_type(TChar), pFrom(pFrom), pTo(pTo)
            {
                data.Char.value1 = x;
            }

            EpsilonNFA_Edge(const char* x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : edge_type(TString), pFrom(pFrom), pTo(pTo)
            {
                data.String.value = x;
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

            inline const bool isFromTo()const
            {
                return (edge_type & TFromTo) == TFromTo;
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
                else if (isFromTo()) return TFromTo;
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

        class _hashEpsilonNFAState
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
            bool                   bEnd;
#ifdef _DEBUG
            uint                   idx;
#endif

            DFA_State(const set<EpsilonNFA_State*>& x) : content(x), bFlag(false), bEnd(false)
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
            union
            {
                struct
                {
                    char value1;
                    char value2;
                }Char;
                struct
                {
                    const char* value;
                    size_t      size;
                }String;
            }data;

            enum Edge_Type
            {
                TUnknown = 0,
                TNot     = 1,
                TFromTo  = 2,
                TChar    = 4,
                TString  = 8
            };

            uchar edge_type;

            DFA_State* pFrom;
            DFA_State* pTo;

            DFA_Edge(const pair<char, char>& x, bool bNot, DFA_State* pFrom, DFA_State* pTo) : pFrom(pFrom), pTo(pTo)
            {
                data.Char.value1 = x.first;
                data.Char.value2 = x.second;
                edge_type = bNot ? (TFromTo | TNot) : TFromTo;
            }

            DFA_Edge(char x, bool bNot, DFA_State* pFrom, DFA_State* pTo) : pFrom(pFrom), pTo(pTo)
            {
                data.Char.value1 = x;
                edge_type = bNot ? (TChar | TNot) : TChar;
            }

            DFA_Edge(const char* x, bool bNot, DFA_State* pFrom, DFA_State* pTo) : pFrom(pFrom), pTo(pTo)
            {
                data.String.value = x;
                data.String.size = strlen(x);
                edge_type = bNot ? (TString | TNot) : TString;
            }

            inline const bool isNot()const
            {
                return (edge_type & TNot) == TNot;
            }

            inline const bool isFromTo()const
            {
                return (edge_type & TFromTo) == TFromTo;
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
                if (isFromTo()) return TFromTo;
                else if (isChar()) return TChar;
                else if (isString()) return TString;
                else return TUnknown;
            }

            inline const bool operator<(const DFA_Edge& x)const
            {
                return (ulong)pFrom + pTo < (ulong)x.pFrom + x.pTo;
            }

            inline const bool operator==(const DFA_Edge& x)const
            {
                return pFrom == x.pFrom && pTo == x.pTo;
            }

            const bool compare(const char*& first, const char* last)const
            {
                if (first == last) return false;
                if (isFromTo())
                {
                    if (compare_fromto(*first))
                    {
                        ++first;
                        return true;
                    }
                    return false;
                }
                else if (isChar())
                {
                    if (compare_char(*first))
                    {
                        ++first;
                        return true;
                    }
                    return false;
                }
                else if (isString())
                {
                    size_t n = 0;
                    if (compare_string(first, last, n))
                    {
                        first += n;
                        return true;
                    }
                    return false;
                }
                return false;
            }

            inline const bool compare_fromto(char c)const
            {
#ifdef _DEBUG
                if (!isFromTo()) throw error<string>("error calling function compare_fromto!", __FILE__, __LINE__);
#endif
                if (isNot()) return c < data.Char.value1 || c > data.Char.value2;
                else return c >= data.Char.value1 && c <= data.Char.value2;
            }

            inline const bool compare_char(char c)const
            {
#ifdef _DEBUG
                if (!isChar()) throw error<string>("error calling function compare_char!", __FILE__, __LINE__);
#endif
                if (isNot()) return c != data.Char.value1;
                else return c == data.Char.value1;
            }

            const bool compare_string(const char* first, const char* last, size_t& n)const
            {
#ifdef _DEBUG
                if (!isString()) throw error<string>("error calling function compare_string!", __FILE__, __LINE__);
#endif
                if (!isNot()) return strncmp(first, data.String.value, max((size_t)(last - first), data.String.size)) == 0;
                return false;
            }
        };

        class _hashDFAState
        {
        public:
            const HASH_KEY_TYPE operator()(const DFA_State* x)const
            {
                return (HASH_KEY_TYPE)x;
            }
        };

        struct EpsilonClosureInfo
        {
            set<EpsilonNFA_State*>             states;
            set<pair<char, bool> >             chars;
            set<pair<const char*, bool> >      strings;
            set<pair<pair<char, char>, bool> > fromtos;

            EpsilonClosureInfo() {}

            EpsilonClosureInfo(const set<EpsilonNFA_State*>& states,
                               const set<pair<char, bool> >& chars,
                               const set<pair<const char*, bool> >& strings,
                               const set<pair<pair<char, char>, bool> >& fromtos)
                               : states(states)
                               , chars(chars)
                               , strings(strings)
                               , fromtos(fromtos) {}

            EpsilonClosureInfo(const EpsilonClosureInfo& x)
            {
                states  = x.states;
                chars   = x.chars;
                strings = x.strings;
                fromtos = x.fromtos;
            }
        };
public:
        struct Context
        {
            set<EpsilonNFA_State*> epsilonNFA_States;
            set<DFA_State*>        dfa_States;

            void clear()
            {
                for (set<EpsilonNFA_State*>::iterator i = epsilonNFA_States.begin(), m = epsilonNFA_States.end(); i != m; ++i)
                {
                    destruct(*i, has_destruct(*(*i)));
                    EpsilonNFA_State_Alloc::deallocate(*i);
                }
                epsilonNFA_States.clear();

                for (set<DFA_State*>::iterator i = dfa_States.begin(), m = dfa_States.end(); i != m; ++i)
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

        struct MatchResult
        {
            const char*  value;
            size_t       pos;

            MatchResult(const char* value, size_t pos) : value(value), pos(pos) {}
            ~MatchResult() {}
        };
    protected:
        typedef allocator<EpsilonNFA_State>  EpsilonNFA_State_Alloc;
        typedef allocator<DFA_State>         DFA_State_Alloc;
    public:
        Rule() : pEpsilonStart(NULL), pEpsilonEnd(NULL), pDFAStart(NULL), pContext(NULL) {}

        Rule(Context* pContext) : pEpsilonStart(NULL), pEpsilonEnd(NULL), pDFAStart(NULL), pContext(pContext) {}

        Rule(char x, Context* pContext) : pDFAStart(NULL), pContext(pContext)
        {
            pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonStart);

            pEpsilonEnd   = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonEnd);

            epsilonNFA_Edges[pEpsilonStart].push_back(EpsilonNFA_Edge(x, pEpsilonStart, pEpsilonEnd));

            pContext->epsilonNFA_States.insert(pEpsilonStart);
            pContext->epsilonNFA_States.insert(pEpsilonEnd);
        }

        Rule(const char* x, Context* pContext) : pDFAStart(NULL), pContext(pContext)
        {
            pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonStart);

            pEpsilonEnd   = EpsilonNFA_State_Alloc::allocate();
            construct(pEpsilonEnd);

            epsilonNFA_Edges[pEpsilonStart].push_back(EpsilonNFA_Edge(x, pEpsilonStart, pEpsilonEnd));

            pContext->epsilonNFA_States.insert(pEpsilonStart);
            pContext->epsilonNFA_States.insert(pEpsilonEnd);
        }

        Rule(const Rule& x) : pContext(x.pContext)
        {
            pEpsilonStart    = x.pEpsilonStart;
            pEpsilonEnd      = x.pEpsilonEnd;
            epsilonNFA_Edges = x.epsilonNFA_Edges;

            pDFAStart = x.pDFAStart;
            pDFAEnds  = x.pDFAEnds;
            dfa_Edges = x.dfa_Edges;
        }

        ~Rule() {}

        Rule operator+(const Rule& x)
        {
            Rule a(pContext), b(x.pContext);
            cloneEpsilonNFA(*this, a);
            cloneEpsilonNFA(x, b);
            copyEpsilonNFA_Edges(b, a);

            a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, b.pEpsilonStart));
            a.pEpsilonEnd = b.pEpsilonEnd;
            return a;
        }

        Rule operator-(const Rule& x)
        {
            if (epsilonNFA_Edges.size() == 1 && x.epsilonNFA_Edges.size() == 1 &&
                epsilonNFA_Edges.begin()->second.size() == 1 && x.epsilonNFA_Edges.begin()->second.size() == 1 &&
                epsilonNFA_Edges.begin()->second.begin()->edge_type == EpsilonNFA_Edge::TChar && x.epsilonNFA_Edges.begin()->second.begin()->edge_type == EpsilonNFA_Edge::TChar)
            {
                Rule a(pContext);
                a.pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
                construct(a.pEpsilonStart);

                a.pEpsilonEnd = EpsilonNFA_State_Alloc::allocate();
                construct(a.pEpsilonEnd);

                pContext->epsilonNFA_States.insert(a.pEpsilonStart);
                pContext->epsilonNFA_States.insert(a.pEpsilonEnd);
                a.epsilonNFA_Edges[a.pEpsilonStart].push_back(EpsilonNFA_Edge(epsilonNFA_Edges.begin()->second.begin()->data.Char.value1, x.epsilonNFA_Edges.begin()->second.begin()->data.Char.value1, a.pEpsilonStart, a.pEpsilonEnd));
                return a;
            }
            else
            {
                throw error<string>("doesn't support", __FILE__, __LINE__);
            }
            return Rule();
        }

        Rule operator|(const Rule& x)
        {
            Rule a(pContext), b(x.pContext);
            cloneEpsilonNFA(*this, a);
            cloneEpsilonNFA(x, b);
            copyEpsilonNFA_Edges(b, a);

            EpsilonNFA_State* _pStart = EpsilonNFA_State_Alloc::allocate();
            construct(_pStart);

            EpsilonNFA_State* _pEnd = EpsilonNFA_State_Alloc::allocate();
            construct(_pEnd);

            pContext->epsilonNFA_States.insert(_pStart);
            pContext->epsilonNFA_States.insert(_pEnd);

            a.epsilonNFA_Edges[_pStart].push_back(EpsilonNFA_Edge(_pStart, a.pEpsilonStart));
            a.epsilonNFA_Edges[_pStart].push_back(EpsilonNFA_Edge(_pStart, b.pEpsilonStart));
            a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, _pEnd));
            a.epsilonNFA_Edges[b.pEpsilonEnd].push_back(EpsilonNFA_Edge(b.pEpsilonEnd, _pEnd));

            a.pEpsilonStart = _pStart;
            a.pEpsilonEnd   = _pEnd;

            return a;
        }

        inline Rule opt()
        {
            Rule a(pContext);
            cloneEpsilonNFA(*this, a);
            a.epsilonNFA_Edges[a.pEpsilonStart].push_back(EpsilonNFA_Edge(a.pEpsilonStart, a.pEpsilonEnd));
            return a;
        }

        Rule operator*()
        {
            Rule a(pContext);
            cloneEpsilonNFA(*this, a);

            a.epsilonNFA_Edges[a.pEpsilonStart].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, a.pEpsilonStart));

            a.pEpsilonEnd = a.pEpsilonStart;

            return a;
        }

        Rule operator+()
        {
            Rule a(pContext);
            cloneEpsilonNFA(*this, a);
            
            a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, a.pEpsilonStart));

            return a;
        }

        Rule operator!()
        {
            Rule a(pContext);
            cloneEpsilonNFA(*this, a);
            for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hashEpsilonNFAState>::iterator i = a.epsilonNFA_Edges.begin(), m = a.epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (vector<EpsilonNFA_Edge>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    if (j->isFromTo() || j->isChar()) j->negate();
                }
            }
            return a;
        }

        inline Rule& operator=(const Rule& x)
        {
            if (&x != this)
            {
                pContext = x.pContext;

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

            if (isEndDFAStatus(pDFAStart))
            {
                pDFAStart->bEnd = true;
                pDFAEnds.insert(pDFAStart);
            }
            pContext->dfa_States.insert(pDFAStart);

            while (!c2.empty())
            {
                DFA_State* t = c2.front().first;
                t->bFlag = true;

                enumerateChars(t, c, c2, c2.front().second.chars);
                enumerateChars(t, c, c2, c2.front().second.strings);
                enumerateChars(t, c, c2, c2.front().second.fromtos);

                c2.pop();
            }
        }

        bool parse(const char* first, const char* last, char*& output, size_t& size)
        {
            const char* f = first;
            typedef pair<DFA_State*, const char*> c_type;
            stack<c_type> s;
            s.push(c_type(pDFAStart, first));
            while (true)
            {
                bool bContinue = false;
                for (vector<DFA_Edge>::const_iterator i = dfa_Edges[s.top().first].begin(), m = dfa_Edges[s.top().first].end(); i != m; ++i)
                {
                    if (i->compare(first, last))
                    {
                        s.push(c_type(i->pTo, first));
                        bContinue = true;
                        break;
                    }
                }
                if (!bContinue) break;
            }
            while (!s.empty() && !s.top().first->bEnd)
            {
                s.pop();
            }
            if (s.empty()) return false;

            size = s.top().second - f;
            return true;
        }

        MatchResult match(const char* first, const char* last, size_t pos = 0)
        {
#ifdef _DEBUG
            if (pos < 0 || pos >= (size_t)(last - first)) throw error<string>("invalid position of input", __FILE__, __LINE__);
#endif
            const char* b = first;

            char*  output;
            size_t size;

            for (const char* i = first + pos; i != last; ++i)
            {
                if (parse(i, last, output, size))
                {
                    return MatchResult(output, i - b);
                }
            }
            return MatchResult(output, -1);
        }

#ifdef _DEBUG
        void printEpsilonNFA()
        {
            printf("-------- ε- NFA Start --------\n");
            for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hashEpsilonNFAState>::const_iterator i = epsilonNFA_Edges.begin(), m = epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    printf("%03d -> %03d", j->pFrom->idx, j->pTo->idx);
                    switch (j->edgeType())
                    {
                    case EpsilonNFA_Edge::TEpsilon:
                        printf("(ε)");
                        break;
                    case EpsilonNFA_Edge::TFromTo:
                        printf("(%c - %c)", j->data.Char.value1, j->data.Char.value2);
                        break;
                    case EpsilonNFA_Edge::TChar:
                        printf("(%c)", j->data.Char.value1);
                        break;
                    case EpsilonNFA_Edge::TString:
                        printf("(%s)", j->data.String.value);
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
            for (hashmap<DFA_State*, vector<DFA_Edge>, _hashDFAState>::const_iterator i = dfa_Edges.begin(), m = dfa_Edges.end(); i != m; ++i)
            {
                for (vector<DFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    printf("%03d -> %03d", j->pFrom->idx, j->pTo->idx);
                    switch (j->edgeType())
                    {
                    case DFA_Edge::TFromTo:
                        printf("(%c - %c)", j->data.Char.value1, j->data.Char.value2);
                        break;
                    case DFA_Edge::TChar:
                        printf("(%c)", j->data.Char.value1);
                        break;
                    case DFA_Edge::TString:
                        printf("(%s)", j->data.String.value);
                        break;
                    default:
                        break;
                    }
                    if (j->isNot()) printf("(not)");
                    printf("\n");
                    tmp.insert(j->pFrom);
                    tmp.insert(j->pTo);
                }
            }

            printf("start: %03d -> ends: ", pDFAStart->idx);
            for (set<DFA_State*>::const_iterator i = pDFAEnds.begin(), m = pDFAEnds.end(); i != m; ++i)
            {
                printf("%03d ", (*i)->idx);
            }
            printf("\n");
#if DEBUG_LEVEL == 3
            printf("-------------------------------\n");

            for (set<DFA_State*>::const_iterator i = tmp.begin(), m = tmp.end(); i != m; ++i)
            {
                printf("State: %03d\n", (*i)->idx);
                for (set<EpsilonNFA_State*>::const_iterator j = (*i)->content.begin(), n = (*i)->content.end(); j != n; ++j)
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
        static void copyEpsilonNFA_Edges(const Rule& from, Rule& to)
        {
            for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hashEpsilonNFAState>::const_iterator i = from.epsilonNFA_Edges.begin(), m = from.epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    to.epsilonNFA_Edges[i->first].push_back(*j);
                }
            }
        }

        static void cloneEpsilonNFA(const Rule& x, Rule& to)
        {
            map<EpsilonNFA_State*, EpsilonNFA_State*> statesMap;
            for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hashEpsilonNFAState>::const_iterator i = x.epsilonNFA_Edges.begin(), m = x.epsilonNFA_Edges.end(); i != m; ++i)
            {
                for (vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
                {
                    EpsilonNFA_State *pFrom = statesMap[j->pFrom], *pTo = statesMap[j->pTo];
                    if (pFrom == NULL)
                    {
                        pFrom = statesMap[j->pFrom] = EpsilonNFA_State_Alloc::allocate();
                        construct(pFrom);

                        to.pContext->epsilonNFA_States.insert(pFrom);
                    }

                    if (pTo == NULL)
                    {
                        pTo = statesMap[j->pTo] = EpsilonNFA_State_Alloc::allocate();
                        construct(pTo);

                        to.pContext->epsilonNFA_States.insert(pTo);
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
            for (set<EpsilonNFA_State*>::const_iterator i = k.begin(), m = k.end(); i != m; ++i)
            {
                info.states.insert(*i);
                set<EpsilonNFA_State*> tmp;
                tmp.insert(*i);
                while (!tmp.empty())
                {
                    EpsilonNFA_State* pState = *tmp.begin();
                    for (vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[pState].begin(), n = epsilonNFA_Edges[pState].end(); j != n; ++j)
                    {
                        if (j->isEpsilon())
                        {
                            if (info.states.insert(j->pTo).second) tmp.insert(j->pTo);
                        }
                        else if (j->isFromTo()) info.fromtos.insert(pair<pair<char, char>, bool>(pair<char, char>(j->data.Char.value1, j->data.Char.value2), j->isNot()));
                        else if (j->isChar()) info.chars.insert(pair<char, bool>(j->data.Char.value1, j->isNot()));
                        else if (j->isString()) info.strings.insert(pair<const char*, bool>(j->data.String.value, j->isNot()));
                    }
                    tmp.erase(pState);
                }
            }
        }

        set<EpsilonNFA_State*> move(const DFA_State& t, const char& c, bool bNot)
        {
            set<EpsilonNFA_State*> result;
            for (set<EpsilonNFA_State*>::const_iterator i = t.content.begin(), m = t.content.end(); i != m; ++i)
            {
                for (vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[*i].begin(), n = epsilonNFA_Edges[*i].end(); j != n; ++j)
                {
                    if (j->isChar() && j->data.Char.value1 == c && j->isNot() == bNot) result.insert(j->pTo);
                }
            }
            return result;
        }

        set<EpsilonNFA_State*> move(const DFA_State& t, const char*& s, bool bNot)
        {
            set<EpsilonNFA_State*> result;
            for (set<EpsilonNFA_State*>::const_iterator i = t.content.begin(), m = t.content.end(); i != m; ++i)
            {
                for (vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[*i].begin(), n = epsilonNFA_Edges[*i].end(); j != n; ++j)
                {
                    if (j->isString() && j->data.String.value == s && j->isNot() == bNot) result.insert(j->pTo);
                }
            }
            return result;
        }

        set<EpsilonNFA_State*> move(const DFA_State& t, const pair<char, char>& p, bool bNot)
        {
            set<EpsilonNFA_State*> result;
            for (set<EpsilonNFA_State*>::const_iterator i = t.content.begin(), m = t.content.end(); i != m; ++i)
            {
                for (vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[*i].begin(), n = epsilonNFA_Edges[*i].end(); j != n; ++j)
                {
                    if (j->isFromTo() && j->data.Char.value1 == p.first && j->data.Char.value2 == p.second && j->isNot() == bNot) result.insert(j->pTo);
                }
            }
            return result;
        }

        DFA_State* getDFAState(const set<EpsilonNFA_State*>& t, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c)
        {
            for (list<pair<DFA_State*, EpsilonClosureInfo> >::const_iterator i = c[t.size()].begin(), m = c[t.size()].end(); i != m; ++i)
            {
                if (i->first->content == t) return i->first;
            }
            return NULL;
        }

        template <typename T1, typename T2>
        void enumerateChars(DFA_State* t, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c, queue<pair<DFA_State*, EpsilonClosureInfo> >& c2, const set<pair<T1, T2> >& container)
        {
            for (typename set<pair<T1, T2> >::const_iterator i = container.begin(), m = container.end(); i != m; ++i)
            {
                EpsilonClosureInfo info;
                epsilonClosure(move(*t, i->first, i->second), info);

                DFA_State* p = getDFAState(info.states, c);
                if (p) // 如果这个状态已存在
                {
                    dfa_Edges[t].push_back(DFA_Edge(i->first, i->second, t, p));
                }
                else
                {
                    DFA_State* pState = DFA_State_Alloc::allocate();
                    construct(pState, info.states);
                    pContext->dfa_States.insert(pState);

                    if (isEndDFAStatus(pState))
                    {
                        pState->bEnd = true;
                        pDFAEnds.insert(pState);
                    }

                    typedef pair<DFA_State*, EpsilonClosureInfo> c_type;
                    c_type ct(pState, info);
                    c[info.states.size()].push_back(ct);
                    c2.push(ct);

                    dfa_Edges[t].push_back(DFA_Edge(i->first, i->second, t, pState));
                }
            }
        }

        template <typename T1, typename T2, typename T3>
        void enumerateChars(DFA_State* t, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c, queue<pair<DFA_State*, EpsilonClosureInfo> >& c2, const set<pair<pair<T1, T2>, T3> >& container)
        {
            for (typename set<pair<pair<T1, T2>, T3> >::const_iterator i = container.begin(), m = container.end(); i != m; ++i)
            {
                EpsilonClosureInfo info;
                epsilonClosure(move(*t, i->first, i->second), info);

                DFA_State* p = getDFAState(info.states, c);
                if (p) // 如果这个状态已存在
                {
                    dfa_Edges[t].push_back(DFA_Edge(i->first, i->second, t, p));
                }
                else
                {
                    DFA_State* pState = DFA_State_Alloc::allocate();
                    construct(pState, info.states);
                    pContext->dfa_States.insert(pState);

                    if (isEndDFAStatus(pState))
                    {
                        pState->bEnd = true;
                        pDFAEnds.insert(pState);
                    }

                    typedef pair<DFA_State*, EpsilonClosureInfo> c_type;
                    c_type ct(pState, info);
                    c[info.states.size()].push_back(ct);
                    c2.push(ct);

                    dfa_Edges[t].push_back(DFA_Edge(i->first, i->second, t, pState));
                }
            }
        }

        const bool isEndDFAStatus(const DFA_State* pState)const
        {
            for (set<EpsilonNFA_State*>::const_iterator i = pState->content.begin(), m = pState->content.end(); i != m; ++i)
            {
                if (*i == pEpsilonEnd) return true;
            }
            return false;
        }
    protected:
        EpsilonNFA_State *pEpsilonStart, *pEpsilonEnd;
        hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge>, _hashEpsilonNFAState> epsilonNFA_Edges;

        DFA_State*      pDFAStart;
        set<DFA_State*> pDFAEnds;
        hashmap<DFA_State*, vector<DFA_Edge>, _hashDFAState> dfa_Edges;

        Context* pContext;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
