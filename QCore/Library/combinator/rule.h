/********************************************************************
	created:	2013/01/19
	created:	19:1:2013   20:29
	filename: 	\QCore\Library\combinator\rule.h
	file path:	\QCore\Library\combinator
	file base:	rule
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_RULE_H_
#define _QLANGUAGE_LIBRARY_COMBINATOR_RULE_H_

#include "../definition.h"
#include "../allocator.h"
#include "../construct.h"

#include "alt.h"
#include "ref.h"
#include "seq.h"
#include "while.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace combinator
{
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorNode
    {
        template <typename A, typename B, typename C, typename D>
        friend class CombinatorRule;

        typedef CombinatorNode<I, O, IOO, E> self;
        typedef size_t                       size_type;
    public:
        CombinatorNode() : pCombinator(NULL), pSize(NULL) {}
        CombinatorNode(Combinator<I, O, IOO, E>* ptr) : pCombinator(ptr), pSize(NULL) { inc(); }
        CombinatorNode(const self& node) : pCombinator(node.pCombinator), pSize(node.pSize) { inc(); }
        virtual ~CombinatorNode() { dec(); }

        inline virtual bool parse(const I& input, O& output)
        {
            if (pCombinator) return pCombinator->parse(input, output);
            return false;
        }

        inline virtual Combinator<I, O, IOO, E>* getCombinator()
        {
            return pCombinator;
        }

        inline self operator+(const self& node)
        {
            typedef allocator<CombinatorSeq<I, O, IOO, E> > Alloc;
            CombinatorSeq<I, O, IOO, E>* pSeq = Alloc::allocate();
            construct(pSeq, getCombinator(), const_cast<self&>(node).getCombinator());
            return self(pSeq);
        }

        inline self operator|(const self& node)
        {
            typedef allocator<CombinatorAlt<I, O, IOO, E> > Alloc;
            CombinatorAlt<I, O, IOO, E>* pAlt = Alloc::allocate();
            construct(pAlt, getCombinator(), const_cast<self&>(node).getCombinator());
            return self(pAlt);
        }

        self& operator=(const self& node)
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
    protected:
        void inc()
        {
            if (pSize) ++*pSize;
            else
            {
                typedef allocator<size_type> Alloc;
                pSize = Alloc::allocate();
                construct(pSize, 1);
            }
        }

        void dec()
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
    protected:
        Combinator<I, O, IOO, E>* pCombinator;
        size_type*                pSize;
    };

    /************************************************************************/
    /* CombinatorRule is the topest interface                               */
    /************************************************************************/
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorRule : public CombinatorNode<I, O, IOO, E>
    {
        typedef CombinatorNode<I, O, IOO, E> parent;
        typedef CombinatorRule<I, O, IOO, E> self;
    public:
        CombinatorRule() : parent()
        {
            typedef allocator<CombinatorRef<I, O, IOO, E> > Alloc;
            pRef = Alloc::allocate();
            construct<CombinatorRef<I, O, IOO, E>, Combinator<I, O, IOO, E>*&>(pRef, parent::pCombinator);
        }

        CombinatorRule(const parent& node) : parent(node), pRef(NULL) {}

        virtual ~CombinatorRule()
        {
            if (pRef)
            {
                const typename Combinator<I, O, IOO, E>::size_type size = pRef->objSize();
                pRef->destruct();
                typedef allocator<void> Alloc;
                Alloc::deallocateWithSize(pRef, size);
                pRef = NULL;
            }
        }

        inline virtual Combinator<I, O, IOO, E>* getCombinator()
        {
            return pRef ? pRef : parent::pCombinator;
        }

        self& operator=(const parent& node)
        {
            this->dec();
            parent::pCombinator = const_cast<parent&>(node).getCombinator();
            parent::pSize = node.pSize;
            this->inc();
            return *this;
        }

        self& operator=(const self& rule)
        {
            this->dec();
            parent::pCombinator = const_cast<self&>(rule).getCombinator();
            parent::pSize = rule.pSize;
            this->inc();
            return *this;
        }
    protected:
        CombinatorRef<I, O, IOO, E>* pRef;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
