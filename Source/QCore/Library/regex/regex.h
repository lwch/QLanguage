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
#include "../iostream.h"
#include "../fstream.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace regex
{
    class Rule
    {
    protected:
        struct Variant
        {
            enum Type
            {
                TUnknown = 0,
                TNot     = 1,
                TFromTo  = 2,
                TChar    = 4,
                TString  = 8,
                TEpsilon = 16
            };

            uchar type;

            union
            {
                struct
                {
                    char value1;
                    char value2;
                }Char;

                struct
                {
                    char*  value;
                    size_t size;
                }String;
            }data;

            Variant();
            Variant(char value, bool bNot);
            Variant(const pair<char, char>& x, bool bNot);
            Variant(const char* value, bool bNot);
            Variant(const Variant& x);
            ~Variant();

            void negate();
            const bool isNot()const;
            const bool isEpsilon()const;
            const bool isFromTo()const;
            const bool isChar()const;
            const bool isString()const;
            const Type trueType()const;

            const bool operator==(const Variant& x)const;
            Variant& operator=(const Variant& x);

            bool output(ostream& stream);
            bool loadFromData(const char*& buffer);
            bool loadFromStream(istream& stream);
        };

        struct EpsilonNFA_State;

        struct EpsilonNFA_Edge
        {
            Variant value;

            EpsilonNFA_State* pFrom;
            EpsilonNFA_State* pTo;

            EpsilonNFA_Edge(EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo);
            EpsilonNFA_Edge(const pair<char, char>& x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo);
            EpsilonNFA_Edge(char x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo);
            EpsilonNFA_Edge(const char* x, EpsilonNFA_State* pFrom, EpsilonNFA_State* pTo);

            void negate();
            const bool isNot()const;
            const bool isEpsilon()const;
            const bool isFromTo()const;
            const bool isChar()const;
            const bool isString()const;
            const Variant::Type edgeType()const;

            const bool operator==(const EpsilonNFA_Edge& x)const;
        };

        struct DFA_State;

        struct DFA_Edge
        {
            Variant value;

            DFA_State* pFrom;
            DFA_State* pTo;

            DFA_Edge();
            DFA_Edge(const Variant& x, DFA_State* pFrom, DFA_State* pTo);

            const bool isNot()const;
            const bool isFromTo()const;
            const bool isChar()const;
            const bool isString()const;
            const Variant::Type edgeType()const;

            const bool operator==(const DFA_Edge& x)const;

            const bool compare(const char*& first, const char* last)const;
            const bool compare_fromto(char c)const;
            const bool compare_char(char c)const;
            const bool compare_string(const char* first, const char* last, size_t& n)const;

            bool output(ostream& stream);
        };

        struct EpsilonClosureInfo
        {
            vector<EpsilonNFA_State*> states;
            vector<Variant>           variants;

            EpsilonClosureInfo();
            EpsilonClosureInfo(const vector<EpsilonNFA_State*>& states, const vector<Variant>& variants);
            EpsilonClosureInfo(const EpsilonClosureInfo& x);
        };
public:
        struct Context
        {
            set<EpsilonNFA_State*> epsilonNFA_States;
            set<DFA_State*>        dfa_States;

            ~Context();
            void clear();
        };

        struct MatchResult
        {
            const char*  value;
            size_t       pos;

            MatchResult(const char* value, size_t pos);
        };
    protected:
        typedef allocator<EpsilonNFA_State>  EpsilonNFA_State_Alloc;
        typedef allocator<DFA_State>         DFA_State_Alloc;
    public:
        Rule();
        Rule(Context* pContext);
        Rule(char x, Context* pContext);
        Rule(const char* x, Context* pContext);
        Rule(const Rule& x);

        ~Rule() {}

#ifdef _DEBUG
        inline void setShowName(const string& showName) { this->showName = showName; }
#endif

        Rule operator+(const Rule& x);
        Rule operator-(const Rule& x);
        Rule operator|(const Rule& x);
        Rule opt();
        Rule operator*();
        Rule operator+();
        Rule operator!();

        Rule& operator=(const Rule& x);

        void buildDFA();
        bool parse(const char* first, const char* last, char*& output, size_t& size);
        MatchResult match(const char* first, const char* last, size_t pos = 0);

        const bool operator==(const Rule& r)const;
        const bool operator!=(const Rule& r)const;

        const bool output(ostream& stream)const;
        bool loadFromData(const char*& data);
        bool loadFromStream(istream& stream);
        void printEpsilonNFA(ostream& stream)const;
        void printDFA(ostream& stream)const;
        void printShowName(ostream& stream)const;
    protected:
        static void copyEpsilonNFA_Edges(const Rule& from, Rule& to);
        static void cloneEpsilonNFA(const Rule& x, Rule& to);

        void epsilonClosure(const vector<EpsilonNFA_State*>& k, EpsilonClosureInfo& info);
        void epsilonClosure(EpsilonNFA_State* pState, EpsilonClosureInfo& info, vector<EpsilonNFA_State*>& v);
        vector<EpsilonNFA_State*> move(const DFA_State& t, const Variant& v);
        DFA_State* getDFAState(const vector<EpsilonNFA_State*>& t, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c);
        void enumerateChars(DFA_State* t, map<size_t, list<pair<DFA_State*, EpsilonClosureInfo> > >& c, queue<pair<DFA_State*, EpsilonClosureInfo> >& c2, const vector<Variant>& container);
        const bool isEndDFAStatus(const DFA_State* pState)const;

        static uint inc()
        {
            static uint i = 0;
            return i++;
        }
    protected:
        EpsilonNFA_State *pEpsilonStart, *pEpsilonEnd;
        hashmap<EpsilonNFA_State*, vector<EpsilonNFA_Edge> > epsilonNFA_Edges;

        DFA_State*      pDFAStart;
        set<DFA_State*> pDFAEnds;
        size_t          dfa_Edges_Count;
        hashmap<DFA_State*, vector<DFA_Edge> > dfa_Edges;

        Context* pContext;
        uint     idx;
    public:
#ifdef _DEBUG
        string   showName;
#endif
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
