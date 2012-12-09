/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:53
	filename: 	\QCore\Library\combinator\combinator.cpp
	file path:	\QCore\Library\combinator
	file base:	combinator
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_COMBINATOR_CPP_
#define _QLANGUAGE_LIBRARY_COMBINATOR_COMBINATOR_CPP_

#include "combinator.h"
#include "seq.cpp"
#include "alt.cpp"
#include "while.cpp"
#include "ref.cpp"

NAMESPACE_QLANGUAGE_LIBRARY_START
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename I, typename O>
    inline const I DefaultCombinatorResultSelector::operator()(const CombinatorResult<O>& result)const
    {
        return result.result;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename I, typename O, typename IOO, typename E>
    CombinatorNode<I, O, IOO, E>::~CombinatorNode()
    {
        if (pCombinator)
        {
            const typename Combinator<I, O, IOO, E>::size_type size = pCombinator->objSize();
            pCombinator->destruct();
            typedef allocator<void> Alloc;
            Alloc::deallocateWithSize(pCombinator, size);
            pCombinator = NULL;
        }
    }

    template <typename I, typename O, typename IOO, typename E>
    inline bool CombinatorNode<I, O, IOO, E>::parse(const I& input, O& output)
    {
        if (pCombinator) return pCombinator->parse(input, output);
        return false;
    }

    template <typename I, typename O, typename IOO, typename E>
    inline CombinatorNode<I, O, IOO, E> CombinatorNode<I, O, IOO, E>::operator+(const self& node)
    {
        typedef allocator<CombinatorSeq<I, O, IOO, E> > Alloc;
        CombinatorSeq<I, O, IOO, E>* pSeq = Alloc::allocate();
        construct(pSeq, pCombinator, node.pCombinator);
        return self(pSeq);
    }

    template <typename I, typename O, typename IOO, typename E>
    inline CombinatorNode<I, O, IOO, E> CombinatorNode<I, O, IOO, E>::operator|(const self& node)
    {
        typedef allocator<CombinatorAlt<I, O, IOO, E> > Alloc;
        CombinatorAlt<I, O, IOO, E>* pAlt = Alloc::allocate();
        construct(pAlt, pCombinator, node.pCombinator);
        return self(pAlt);
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
