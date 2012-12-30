/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:53
	filename: 	\QCore\Library\combinator\combinator.h
	file path:	\QCore\Library\combinator
	file base:	combinator
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_COMBINATOR_H_
#define _QLANGUAGE_LIBRARY_COMBINATOR_COMBINATOR_H_

#include "../definition.h"
#include "../allocator.h"
#include "../construct.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename O>
    class CombinatorResult
    {
    public:
        CombinatorResult() : bResult(false) {}
        CombinatorResult(bool bResult, const O& result) : bResult(bResult), result(result) {}
        virtual ~CombinatorResult() {}
    public:
        bool bResult;
        O result;
    };

    class DefaultCombinatorResultSelector
    {
    public:
        template <typename I, typename O>
        const I operator()(const CombinatorResult<O>& result)const;
    };

    /************************************************************************/
    /* I: Input Type                                                        */
    /* O: Output Type                                                       */
    /* IOO: InputOfOutput                                                   */
    /* E: End of input                                                      */
    /************************************************************************/
    template <typename I, typename O, typename IOO, typename E>
    class Combinator
    {
    public:
        Combinator() {}
        virtual ~Combinator() {}

        virtual bool parse(const I& input, O& output)=0;

        virtual void destruct()=0;

        typedef size_t size_type;
        virtual const size_type objSize()const=0;
    protected:
        IOO input;
        E   end;
    };

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

        virtual bool parse(const I& input, O& output);
        virtual Combinator<I, O, IOO, E>* getCombinator();

        self operator+(const self& node);
        self operator|(const self& node);
        self& operator=(const self& node);
    protected:
        void inc();
        void dec();
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
        CombinatorRule() : parent() {}
        CombinatorRule(const parent& node) : parent(node) {}
        virtual ~CombinatorRule() {}

        virtual Combinator<I, O, IOO, E>* getCombinator();

        self& operator=(const parent& node);
        self& operator=(const self& rule);
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
