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

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename I, typename O>
    inline const I DefaultCombinatorResultSelector::operator()(const CombinatorResult<O>& result)const
    {
        return result.result;
    }

    template <typename I, typename O, typename IOO>
    inline bool CombinatorNode<I, O, IOO>::parse(const I& input, O& output)
    {
        if (pCombinator) return pCombinator->parse(input, output);
        return false;
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif