/********************************************************************
	created:	2013/05/08
	created:	8:5:2013   16:47
	filename: 	\QCore\Library\regex\regex.cpp
	file path:	\QCore\Library\regex
	file base:	regex
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "regex.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace regex
{
    // Rule::Variant
    Rule::Variant::Variant() : type(TEpsilon)
    {
    }

    Rule::Variant::Variant(char value, bool bNot)
    {
        memset(&data, 0, sizeof(data));
        data.Char.value1 = value;
        type = bNot ? (TChar | TNot) : TChar;
    }

    Rule::Variant::Variant(const pair<char, char>& x, bool bNot)
    {
        memset(&data, 0, sizeof(data));
        data.Char.value1 = x.first;
        data.Char.value2 = x.second;
        type = bNot ? (TFromTo | TNot) : TFromTo;
    }

    Rule::Variant::Variant(const char* value, bool bNot)
    {
        memset(&data, 0, sizeof(data));
        data.String.size  = strlen(value);
        data.String.value = allocator<char>::allocate(data.String.size + 1);
        strcpy(data.String.value, value);
        data.String.value[data.String.size] = 0;
        type = bNot ? (TString | TNot) : TString;
    }

    Rule::Variant::Variant(const Variant& x)
    {
        type = x.type;
        if (isString())
        {
            memset(&data, 0, sizeof(data));
            data.String.size  = x.data.String.size;
            data.String.value = allocator<char>::allocate(x.data.String.size + 1);
            strcpy(data.String.value, x.data.String.value);
            data.String.value[data.String.size] = 0;
        }
        else data = x.data;
    }

    Rule::Variant::~Variant()
    {
        if (isString() && data.String.value)
        {
            destruct(data.String.value, data.String.value + data.String.size + 1);
            allocator<char>::deallocate(data.String.value, data.String.size + 1);
        }
    }

    void Rule::Variant::negate()
    {
        type ^= TNot;
    }

    const bool Rule::Variant::isNot()const
    {
        return (type & TNot) == TNot;
    }

    const bool Rule::Variant::isEpsilon()const
    {
        return (type & TEpsilon) == TEpsilon;
    }

    const bool Rule::Variant::isFromTo()const
    {
        return (type & TFromTo) == TFromTo;
    }

    const bool Rule::Variant::isChar()const
    {
        return (type & TChar) == TChar;
    }

    const bool Rule::Variant::isString()const
    {
        return (type & TString) == TString;
    }

    const Rule::Variant::Type Rule::Variant::trueType()const
    {
        if (isEpsilon()) return TEpsilon;
        else if (isFromTo()) return TFromTo;
        else if (isChar()) return TChar;
        else if (isString()) return TString;
        else return TUnknown;
    }

    const bool Rule::Variant::operator==(const Variant& x)const
    {
        if (type != x.type) return false;
        switch (trueType())
        {
        case TFromTo:
            return data.Char.value1 == x.data.Char.value1 && data.Char.value2 == x.data.Char.value2;
        case TChar:
            return data.Char.value1 == x.data.Char.value1;
        case TString:
            {
                if (data.String.size != x.data.String.size) return false;
                for (size_t i = 0, m = data.String.size; i < m; ++i)
                {
                    if (data.String.value[i] != x.data.String.value[i]) return false;
                }
                return true;
            }
        default:
            return false;
        }
    }

    bool Rule::Variant::output(ostream& stream)
    {
        switch (trueType())
        {
        case TFromTo:
            stream << (uchar)TFromTo << data.Char.value1 << data.Char.value2;
            return true;
        case TChar:
            stream << (uchar)TChar << data.Char.value1;
            return true;
        case TString:
            stream << (uchar)TString << data.String.size << data.String.value;
            return true;
        default:
            return false;
        }
    }
    // Rule::EpsilonNFA_State
    struct Rule::EpsilonNFA_State
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
    // Rule::EpsilonNFA_Edge
    Rule::EpsilonNFA_Edge::EpsilonNFA_Edge(EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : pFrom(pFrom), pTo(pTo)
    {
    }

    Rule::EpsilonNFA_Edge::EpsilonNFA_Edge(const pair<char, char>& x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : value(x, false), pFrom(pFrom), pTo(pTo)
    {
    }

    Rule::EpsilonNFA_Edge::EpsilonNFA_Edge(char x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : value(x, false), pFrom(pFrom), pTo(pTo)
    {
    }

    Rule::EpsilonNFA_Edge::EpsilonNFA_Edge(const char* x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo) : value(x, false), pFrom(pFrom), pTo(pTo)
    {
    }
    // Rule::DFA_State
    struct Rule::DFA_State
    {
        vector<EpsilonNFA_State*> content;
        bool                   bFlag;
        bool                   bEnd;
        uint                   idx;

        DFA_State(const vector<EpsilonNFA_State*>& x) : content(x), bFlag(false), bEnd(false), idx(inc())
        {
        }

        inline const bool operator==(const DFA_State& x)const
        {
            if (&x == this) return true;

            return content == x.content;
        }

        inline uint inc()
        {
            static uint i = 0;
            return i++;
        }
    };
    // Rule::DFA_Edge
    Rule::DFA_Edge::DFA_Edge(const Variant& x, DFA_State* pFrom, DFA_State* pTo) : value(x), pFrom(pFrom), pTo(pTo)
    {
    }

    const bool Rule::DFA_Edge::compare(const char*& first, const char* last)const
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

    const bool Rule::DFA_Edge::compare_string(const char* first, const char* last, size_t& n)const
    {
#ifdef _DEBUG
        if (!isString()) throw error<const char*>("error calling function compare_string!", __FILE__, __LINE__);
#endif
        if (!isNot())
        {
            if ((size_t)(last - first) < value.data.String.size) return false;
            n = value.data.String.size;
            return strncmp(first, value.data.String.value, n) == 0;
        }
        return false;
    }

    bool Rule::DFA_Edge::output(ostream& stream)
    {
        stream << pFrom->idx << pTo->idx;
        return value.output(stream);
    }
    // Rule::EpsilonClosureInfo
    Rule::EpsilonClosureInfo::EpsilonClosureInfo()
    {
    }

    Rule::EpsilonClosureInfo::EpsilonClosureInfo(const vector<EpsilonNFA_State*>& states, const vector<Variant>& variants) : states(states), variants(variants)
    {
    }

    Rule::EpsilonClosureInfo::EpsilonClosureInfo(const EpsilonClosureInfo& x)
    {
        states   = x.states;
        variants = x.variants;
    }
    // Rule::Context
    void Rule::Context::clear()
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

    Rule::Context::~Context()
    {
        clear();
    }
    // Rule::MatchResult
    Rule::MatchResult::MatchResult(const char* value, size_t pos) : value(value), pos(pos)
    {
    }
    // Rule
    Rule::Rule() : pEpsilonStart(NULL), pEpsilonEnd(NULL), pDFAStart(NULL), dfa_Edges_Count(0), pContext(NULL), idx(-1)
    {
    }

    Rule::Rule(Context* pContext) : pEpsilonStart(NULL), pEpsilonEnd(NULL), pDFAStart(NULL), dfa_Edges_Count(0), pContext(pContext), idx(-1)
    {
    }

    Rule::Rule(char x, Context* pContext) : pDFAStart(NULL), dfa_Edges_Count(0), pContext(pContext), idx(inc())
    {
        pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
        construct(pEpsilonStart);

        pEpsilonEnd   = EpsilonNFA_State_Alloc::allocate();
        construct(pEpsilonEnd);

        epsilonNFA_Edges[pEpsilonStart].push_back(EpsilonNFA_Edge(x, pEpsilonStart, pEpsilonEnd));

        pContext->epsilonNFA_States.insert(pEpsilonStart);
        pContext->epsilonNFA_States.insert(pEpsilonEnd);
    }

    Rule::Rule(const char* x, Context* pContext) : pDFAStart(NULL), dfa_Edges_Count(0), pContext(pContext), idx(inc())
    {
        pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
        construct(pEpsilonStart);

        pEpsilonEnd   = EpsilonNFA_State_Alloc::allocate();
        construct(pEpsilonEnd);

        epsilonNFA_Edges[pEpsilonStart].push_back(EpsilonNFA_Edge(x, pEpsilonStart, pEpsilonEnd));

        pContext->epsilonNFA_States.insert(pEpsilonStart);
        pContext->epsilonNFA_States.insert(pEpsilonEnd);
    }

    Rule::Rule(const Rule& x) : pContext(x.pContext)
    {
        pEpsilonStart    = x.pEpsilonStart;
        pEpsilonEnd      = x.pEpsilonEnd;
        epsilonNFA_Edges = x.epsilonNFA_Edges;

        pDFAStart       = x.pDFAStart;
        pDFAEnds        = x.pDFAEnds;
        dfa_Edges_Count = x.dfa_Edges_Count;
        dfa_Edges       = x.dfa_Edges;

        idx = x.idx;
#ifdef _DEBUG
        showName = x.showName;
#endif
    }

    Rule Rule::operator+(const Rule& x)
    {
        Rule a(pContext), b(x.pContext);
        cloneEpsilonNFA(*this, a);
        cloneEpsilonNFA(x, b);
        copyEpsilonNFA_Edges(b, a);

        a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, b.pEpsilonStart));
        a.pEpsilonEnd = b.pEpsilonEnd;
        return a;
    }

    Rule Rule::operator-(const Rule& x)
    {
        if (epsilonNFA_Edges.size() == 1 && x.epsilonNFA_Edges.size() == 1 &&
            epsilonNFA_Edges.begin()->second.size() == 1 && x.epsilonNFA_Edges.begin()->second.size() == 1 &&
            epsilonNFA_Edges.begin()->second.begin()->value.type == Variant::TChar && x.epsilonNFA_Edges.begin()->second.begin()->value.type == Variant::TChar)
        {
            Rule a(pContext);
            a.pEpsilonStart = EpsilonNFA_State_Alloc::allocate();
            construct(a.pEpsilonStart);

            a.pEpsilonEnd = EpsilonNFA_State_Alloc::allocate();
            construct(a.pEpsilonEnd);

            pContext->epsilonNFA_States.insert(a.pEpsilonStart);
            pContext->epsilonNFA_States.insert(a.pEpsilonEnd);
            a.epsilonNFA_Edges[a.pEpsilonStart].push_back(EpsilonNFA_Edge(pair<char, char>(epsilonNFA_Edges.begin()->second.begin()->value.data.Char.value1, x.epsilonNFA_Edges.begin()->second.begin()->value.data.Char.value1), a.pEpsilonStart, a.pEpsilonEnd));
            return a;
        }
        else
        {
            throw error<const char*>("doesn't support", __FILE__, __LINE__);
        }
        return Rule();
    }

    Rule Rule::operator|(const Rule& x)
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

        a.epsilonNFA_Edges[_pStart].push_back_unique(EpsilonNFA_Edge(_pStart, a.pEpsilonStart));
        a.epsilonNFA_Edges[_pStart].push_back_unique(EpsilonNFA_Edge(_pStart, b.pEpsilonStart));
        a.epsilonNFA_Edges[a.pEpsilonEnd].push_back_unique(EpsilonNFA_Edge(a.pEpsilonEnd, _pEnd));
        a.epsilonNFA_Edges[b.pEpsilonEnd].push_back_unique(EpsilonNFA_Edge(b.pEpsilonEnd, _pEnd));

        a.pEpsilonStart = _pStart;
        a.pEpsilonEnd   = _pEnd;

        return a;
    }

    Rule Rule::operator*()
    {
        Rule a(pContext);
        cloneEpsilonNFA(*this, a);

        a.epsilonNFA_Edges[a.pEpsilonStart].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, a.pEpsilonStart));

        a.pEpsilonEnd = a.pEpsilonStart;

        return a;
    }

    Rule Rule::operator+()
    {
        Rule a(pContext);
        cloneEpsilonNFA(*this, a);

        a.epsilonNFA_Edges[a.pEpsilonEnd].push_back(EpsilonNFA_Edge(a.pEpsilonEnd, a.pEpsilonStart));

        return a;
    }

    Rule Rule::operator!()
    {
        Rule a(pContext);
        cloneEpsilonNFA(*this, a);
        for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge> >::iterator i = a.epsilonNFA_Edges.begin(), m = a.epsilonNFA_Edges.end(); i != m; ++i)
        {
            for (vector<EpsilonNFA_Edge>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                if (j->isFromTo() || j->isChar()) j->negate();
            }
        }
        return a;
    }

    void Rule::buildDFA()
    {
        vector<EpsilonNFA_State*> start;
        start.push_back(pEpsilonStart);

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

            enumerateChars(t, c, c2, c2.front().second.variants);

            c2.pop();
        }
    }

    bool Rule::parse(const char* first, const char* last, char*& output, size_t& size)
    {
        output = const_cast<char*>(first);
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
        stack<c_type>::size_type sz = s.size();
        while (sz > 1 && !s.top().first->bEnd)
        {
            s.pop();
            --sz;
        }
        if (sz <= 1)
        {
            output = NULL;
            size   = 0;
            return false;
        }

        size = s.top().second - output;
        return true;
    }

    Rule::MatchResult Rule::match(const char* first, const char* last, size_t pos /* = 0 */)
    {
#ifdef _DEBUG
        if (pos < 0 || pos >= (size_t)(last - first)) throw error<const char*>("invalid position of input", __FILE__, __LINE__);
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

    bool Rule::output(ostream& stream)
    {
        stream << dfa_Edges_Count;
        for (hashmap<DFA_State*, vector<DFA_Edge> >::const_iterator i = dfa_Edges.begin(), m = dfa_Edges.end(); i != m; ++i)
        {
            for (vector<DFA_Edge>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                if (!j->output(stream)) return false;
            }
        }
        stream << pDFAStart->idx;
        stream << pDFAEnds.size();
        for (set<DFA_State*>::const_iterator i = pDFAEnds.begin(), m = pDFAEnds.end(); i != m; ++i)
        {
            stream << (*i)->idx;
        }
        return true;
    }

    void Rule::printEpsilonNFA(ostream& stream)const
    {
#ifdef _DEBUG
        stream << "-------- ε- NFA Start --------" << endl;
        for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge> >::const_iterator i = epsilonNFA_Edges.begin(), m = epsilonNFA_Edges.end(); i != m; ++i)
        {
            for (vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                stream << string::format("%03d -> %03d", j->pFrom->idx, j->pTo->idx);
                switch (j->edgeType())
                {
                case Variant::TEpsilon:
                    stream << "(ε)";
                    break;
                case Variant::TFromTo:
                    stream << string::format("(%c - %c)", j->value.data.Char.value1, j->value.data.Char.value2);
                    break;
                case Variant::TChar:
                    stream << string::format("(%c)", j->value.data.Char.value1);
                    break;
                case Variant::TString:
                    stream << string::format("(%s)", j->value.data.String.value);
                    break;
                default:
                    break;
                }
                if (j->isNot()) stream << "(not)";
                stream << endl;
            }
        }
        if (pEpsilonStart && pEpsilonEnd) stream << string::format("start: %03d -> end: %03d", pEpsilonStart->idx, pEpsilonEnd->idx) << endl;
        stream << "--------- ε- NFA End ---------" << endl;
#endif
    }

    void Rule::printDFA(ostream& stream)const
    {
#ifdef _DEBUG
        stream << "---------- DFA Start ----------" << endl;
        set<DFA_State*> tmp;
        for (hashmap<DFA_State*, vector<DFA_Edge> >::const_iterator i = dfa_Edges.begin(), m = dfa_Edges.end(); i != m; ++i)
        {
            for (vector<DFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                stream << string::format("%03d -> %03d", j->pFrom->idx, j->pTo->idx);
                switch (j->edgeType())
                {
                case Variant::TFromTo:
                    stream << string::format("(%c - %c)", j->value.data.Char.value1, j->value.data.Char.value2);
                    break;
                case Variant::TChar:
                    stream << string::format("(%c)", j->value.data.Char.value1);
                    break;
                case Variant::TString:
                    stream << string::format("(%s)", j->value.data.String.value);
                    break;
                default:
                    break;
                }
                if (j->isNot()) stream << "(not)";
                stream << endl;
                tmp.insert(j->pFrom);
                tmp.insert(j->pTo);
            }
        }

        stream << string::format("start: %03d -> ends: ", pDFAStart->idx);
        for (set<DFA_State*>::const_iterator i = pDFAEnds.begin(), m = pDFAEnds.end(); i != m; ++i)
        {
            stream << string::format("%03d ", (*i)->idx);
        }
        stream << endl;
#if DEBUG_LEVEL == 3
        stream << ("-------------------------------") << endl;

        for (set<DFA_State*>::const_iterator i = tmp.begin(), m = tmp.end(); i != m; ++i)
        {
            stream << string::format("State: %03d", (*i)->idx) << endl;
            for (vector<EpsilonNFA_State*>::const_iterator j = (*i)->content.begin(), n = (*i)->content.end(); j != n; ++j)
            {
                stream << string::format("%03d ", (*j)->idx);
            }
            stream << endl;
        }
#endif
        stream << "----------- DFA End -----------" << endl;
#endif
    }

    void Rule::printShowName(ostream& stream)const
    {
#if defined(_DEBUG) && DEBUG_LEVEL == 3
        stream << showName;
#else
        stream << "Rule";
#endif
    }

    void Rule::copyEpsilonNFA_Edges(const Rule& from, Rule& to)
    {
        for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge> >::const_iterator i = from.epsilonNFA_Edges.begin(), m = from.epsilonNFA_Edges.end(); i != m; ++i)
        {
            for (vector<EpsilonNFA_Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                to.epsilonNFA_Edges[i->first].push_back(*j);
            }
        }
    }

    void Rule::cloneEpsilonNFA(const Rule& x, Rule& to)
    {
        map<EpsilonNFA_State*, EpsilonNFA_State*> statesMap;
        for (hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge> >::const_iterator i = x.epsilonNFA_Edges.begin(), m = x.epsilonNFA_Edges.end(); i != m; ++i)
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

    void Rule::epsilonClosure(const vector<EpsilonNFA_State*>& k, EpsilonClosureInfo& info)
    {
        vector<EpsilonNFA_State*> v;
        for (vector<EpsilonNFA_State*>::const_iterator i = k.begin(), m = k.end(); i != m; ++i)
        {
            epsilonClosure(*i, info, v);
        }
    }

    void Rule::epsilonClosure(EpsilonNFA_State* pState, EpsilonClosureInfo& info, vector<EpsilonNFA_State*>& v)
    {
        if (info.states.push_back_unique(pState))
        {
            for (vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[pState].begin(), n = epsilonNFA_Edges[pState].end(); j != n; ++j)
            {
                if (j->isEpsilon() && v.push_back_unique(j->pTo)) epsilonClosure(j->pTo, info, v);
                else info.variants.push_back_unique(j->value);
            }
        }
    }

    vector<Rule::EpsilonNFA_State*> Rule::move(const DFA_State& t, const Variant& v)
    {
        vector<EpsilonNFA_State*> result;
        for (vector<EpsilonNFA_State*>::const_iterator i = t.content.begin(), m = t.content.end(); i != m; ++i)
        {
            for (vector<EpsilonNFA_Edge>::const_iterator j = epsilonNFA_Edges[*i].begin(), n = epsilonNFA_Edges[*i].end(); j != n; ++j)
            {
                if (j->value == v) result.push_back_unique(j->pTo);
            }
        }
        return result;
    }

    Rule::DFA_State* Rule::getDFAState(const vector<EpsilonNFA_State*>& t, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c)
    {
        for (list<pair<DFA_State*, EpsilonClosureInfo> >::const_iterator i = c[t.size()].begin(), m = c[t.size()].end(); i != m; ++i)
        {
            if (i->first->content == t) return i->first;
        }
        return NULL;
    }

    void Rule::enumerateChars(DFA_State* t, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c, queue<pair<DFA_State*, EpsilonClosureInfo> >& c2, const vector<Variant>& container)
    {
        for (vector<Variant>::const_iterator i = container.begin(), m = container.end(); i != m; ++i)
        {
            EpsilonClosureInfo info;
            epsilonClosure(move(*t, *i), info);

            DFA_State* p = getDFAState(info.states, c);
            if (p) // 如果这个状态已存在
            {
                dfa_Edges[t].push_back(DFA_Edge(*i, t, p));
                ++dfa_Edges_Count;
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

                dfa_Edges[t].push_back(DFA_Edge(*i, t, pState));
                ++dfa_Edges_Count;
            }
        }
    }

    const bool Rule::isEndDFAStatus(const DFA_State* pState)const
    {
        for (vector<EpsilonNFA_State*>::const_iterator i = pState->content.begin(), m = pState->content.end(); i != m; ++i)
        {
            if (*i == pEpsilonEnd) return true;
        }
        return false;
    }
}
NAMESPACE_QLANGUAGE_LIBRARY_END
