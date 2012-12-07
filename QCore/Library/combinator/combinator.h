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
    /************************************************************************/
    template <typename I, typename O, typename IOO>
    class Combinator
    {
    public:
        Combinator() {}
        virtual ~Combinator() {}

        virtual bool parse(const I& input, O& output)=0;
    protected:
        IOO input;
    };

    /************************************************************************/
    /* CombinatorNode is the topest interface                               */
    /************************************************************************/
    template <typename I, typename O, typename IOO>
    class CombinatorNode
    {
    public:
        CombinatorNode() : pCombinator(NULL) {}
        CombinatorNode(const Combinator<I, O, IOO>* pCombinator) : pCombinator(pCombinator) {}
        CombinatorNode(const CombinatorNode<I, O, IOO>& node) : pCombinator(node.pCombinator) {}
        virtual ~CombinatorNode() {}

        virtual bool parse(const I& input, O& output);
    protected:
        Combinator<I, O, IOO>* pCombinator;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
