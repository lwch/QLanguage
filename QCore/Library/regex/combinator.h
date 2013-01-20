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

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace regex
{
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorSeq : public combinator::CombinatorSeq<I, O, IOO, E>
    {
        typedef combinator::CombinatorSeq<I, O, IOO, E> parent;
        typedef combinator::Combinator<I, O, IOO, E> combinator_type;
    public:
        CombinatorSeq(combinator_type* first, combinator_type* second) : parent(first, second) {}
        virtual ~CombinatorSeq() {}

        virtual inline bool hook(const combinator_type* pCombinator)
        {
            return false;
        }
    };

    template <typename I, typename O, typename IOO, typename E>
    class CombinatorNode : public combinator::CombinatorNode<I, O, IOO, E>
    {
        typedef CombinatorNode<I, O, IOO, E> self;
        typedef combinator::CombinatorNode<I, O, IOO, E> parent;
    public:
        CombinatorNode() : parent() {}
        CombinatorNode(combinator::Combinator<I, O, IOO, E>* ptr) : parent(ptr) {}
        CombinatorNode(const self& node) : parent(node) {}
        virtual ~CombinatorNode() {}

        inline self operator+(const self& node)
        {
            typedef allocator<CombinatorSeq<I, O, IOO, E> > Alloc;
            CombinatorSeq<I, O, IOO, E>* pSeq = Alloc::allocate();
            construct(pSeq, this->getCombinator(), const_cast<self&>(node).getCombinator());
            return self(pSeq);
        }
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
