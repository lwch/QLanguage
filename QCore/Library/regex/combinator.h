/********************************************************************
	created:	2013/01/20
	created:	20:1:2013   17:18
	filename: 	\QCore\Library\regex\combinator.h
	file path:	\QCore\Library\regex
	file base:	combinator
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_REGEX_COMBINATOR_H_
#define _QLANGUAGE_LIBRARY_REGEX_COMBINATOR_H_

#include "../definition.h"

#include "../combinator/alt.h"
#include "../combinator/rule.h"
#include "../combinator/seq.h"
#include "../combinator/while.h"

#include "../function.h"
#include "../vector.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace regex
{
    template <typename Char_Type, typename String_Type>
    class CombinatorNFAContext
    {
        struct State
        {
            struct
            {
                Char_Type   char_value;
                String_Type string_value;
            }data;

            enum { TEpsilon, TChar, TString } edge_type;
            enum { TStart, TMiddle, TEnd } state_type;

            vector<State*> to;
        };

        struct StateListNode
        {
            State* pState;
            StateListNode* pNext;
        };

        typedef CombinatorNFAContext<Char_Type, String_Type> self;
        typedef allocator<State> State_Alloc;
        typedef allocator<StateListNode> Node_Alloc;
    public:
        CombinatorNFAContext() : pEnd(NULL)
        {
            pStart = State_Alloc::allocate();
            construct(pStart);
            pStart->state_type = State::TStart;

            pStateList = Node_Alloc::allocate();
            pStateList->pState = pStart;
            pStateList->pNext = NULL;
        }

        virtual ~CombinatorNFAContext() {}

        void push_char(const Char_Type& x)
        {
            State* pState = State_Alloc::allocate();
            construct(pState);
            pState->state_type = State::TEnd;

            StateListNode* pNode = Node_Alloc::allocate();
            pNode->pState = pState;
            pNode->pNext  = pStateList;
            pStateList    = pNode;

            State* pCurrent           = pEnd ? pEnd : pStart;
            pCurrent->edge_type       = State::TChar;
            pCurrent->data.char_value = x;
            pCurrent->to.push_back(pState);

            if (pEnd) pEnd->state_type = State::TMiddle;

            pEnd = pState;
        }

        void clear()
        {
            while (pStateList)
            {
                StateListNode* pNode = pStateList;
                pStateList = pStateList->pNext;
                destruct(pNode->pState, has_destruct(*pNode->pState));
                State_Alloc::deallocate(pNode->pState);
                Node_Alloc::deallocate(pNode);
            }
        }
    protected:
        State *pStart, *pEnd;
        StateListNode* pStateList;
    };

    template <typename Char_Type, typename I, typename O, typename IOO, typename E>
    class CombinatorCh : public combinator::Combinator<I, O, IOO, E>
    {
        typedef combinator::Combinator<I, O, IOO, E> combinator_type;
    public:
        CombinatorCh(const Char_Type& x) : value(x) {}

        virtual inline bool parse(const I& input, O& output)
        {
            output.push_char(value);
            return true;
        }

        RELEASE_ACHIEVE
    protected:
        const Char_Type value;
    };

    template <typename I, typename O, typename IOO, typename E>
    class CombinatorRule : public combinator::CombinatorRule<I, O, IOO, E>
    {
        typedef combinator::CombinatorRule<I, O, IOO, E> parent;
        typedef CombinatorRule<I, O, IOO, E> self;
        typedef combinator::CombinatorNode<I, O, IOO, E> node;
    public:
        CombinatorRule(const node& x) : parent(x) {}

        template <typename Char_Type>
        CombinatorRule(CombinatorCh<Char_Type, I, O, IOO, E>& x) : parent()
        {
            this->pRef->ref = &x;
        }

        virtual ~CombinatorRule() {}

        template <typename Char_Type>
        inline self& operator=(const CombinatorCh<Char_Type, I, O, IOO, E>& x)
        {
            this->dec();
            this->pCombinator = this->pRef;
            this->pSize = this->pSize;
            this->inc();
            return *this;
        }

        inline self& operator=(const node& x)
        {
            return parent::operator=(x);
        }
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
