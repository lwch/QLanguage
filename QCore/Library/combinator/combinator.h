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

    /************************************************************************/
    /* CombinatorNode is the topest interface                               */
    /************************************************************************/
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorNode
    {
        typedef CombinatorNode<I, O, IOO, E> self;
    public:
        CombinatorNode() : pCombinator(NULL) {}
        CombinatorNode(Combinator<I, O, IOO, E>* pCombinator) : pCombinator(pCombinator) {}
        CombinatorNode(const self& node) : pCombinator(node.pCombinator) {}
        virtual ~CombinatorNode();

        virtual bool parse(const I& input, O& output);

        self operator+(const self& node);
        self operator|(const self& node);
    protected:
        Combinator<I, O, IOO, E>* pCombinator;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
