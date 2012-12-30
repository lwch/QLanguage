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
    inline bool CombinatorNode<I, O, IOO, E>::parse(const I& input, O& output)
    {
        if (pCombinator) return pCombinator->parse(input, output);
        return false;
    }

    template <typename I, typename O, typename IOO, typename E>
    inline Combinator<I, O, IOO, E>* CombinatorNode<I, O, IOO, E>::getCombinator()
    {
        return pCombinator;
    }

    template <typename I, typename O, typename IOO, typename E>
    inline CombinatorNode<I, O, IOO, E> CombinatorNode<I, O, IOO, E>::operator+(const self& node)
    {
        typedef allocator<CombinatorSeq<I, O, IOO, E> > Alloc;
        CombinatorSeq<I, O, IOO, E>* pSeq = Alloc::allocate();
        construct(pSeq, getCombinator(), const_cast<self&>(node).getCombinator());
        return self(pSeq);
    }

    template <typename I, typename O, typename IOO, typename E>
    inline CombinatorNode<I, O, IOO, E> CombinatorNode<I, O, IOO, E>::operator|(const self& node)
    {
        typedef allocator<CombinatorAlt<I, O, IOO, E> > Alloc;
        CombinatorAlt<I, O, IOO, E>* pAlt = Alloc::allocate();
        construct(pAlt, getCombinator(), const_cast<self&>(node).getCombinator());
        return self(pAlt);
    }

    template <typename I, typename O, typename IOO, typename E>
    inline CombinatorNode<I, O, IOO, E>& CombinatorNode<I, O, IOO, E>::operator=(const self& node)
    {
        if (pCombinator != node.pCombinator)
        {
            dec();
            pCombinator = node.pCombinator;
            pSize = node.pSize;
            inc();
        }
        return *this;
    }

    template <typename I, typename O, typename IOO, typename E>
    inline void CombinatorNode<I, O, IOO, E>::inc()
    {
        if (pSize) ++*pSize;
        else
        {
            typedef allocator<size_type> Alloc;
            pSize = Alloc::allocate();
            construct(pSize, 1);
        }
    }

    template <typename I, typename O, typename IOO, typename E>
    void CombinatorNode<I, O, IOO, E>::dec()
    {
        if (pSize && --*pSize <= 0 && pCombinator)
        {
            const typename Combinator<I, O, IOO, E>::size_type size = pCombinator->objSize();
            pCombinator->destruct();
            typedef allocator<void> Alloc;
            Alloc::deallocateWithSize(pCombinator, size);
            pCombinator = NULL;
            
            typedef allocator<size_type> size_allocator;
            destruct(pSize, has_destruct(pSize));
            size_allocator::deallocate(pSize);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename I, typename O, typename IOO, typename E>
    inline Combinator<I, O, IOO, E>* CombinatorRule<I, O, IOO, E>::getCombinator()
    {
        // memory leaked
        typedef allocator<CombinatorRef<I, O, IOO, E> > Alloc;
        CombinatorRef<I, O, IOO, E>* pRef = Alloc::allocate();
        construct<CombinatorRef<I, O, IOO, E>, Combinator<I, O, IOO, E>*&>(pRef, parent::pCombinator);
        return pRef;
    }

    template <typename I, typename O, typename IOO, typename E>
    CombinatorRule<I, O, IOO, E>& CombinatorRule<I, O, IOO, E>::operator=(const parent& node)
    {
        dec();
        parent::pCombinator = const_cast<parent&>(node).getCombinator();
        parent::pSize = node.pSize;
        inc();
        return *this;
    }

    template <typename I, typename O, typename IOO, typename E>
    CombinatorRule<I, O, IOO, E>& CombinatorRule<I, O, IOO, E>::operator=(const self& rule)
    {
        dec();
        parent::pCombinator = const_cast<self&>(rule).getCombinator();
        parent::pSize = rule.pSize;
        inc();
        return *this;
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
