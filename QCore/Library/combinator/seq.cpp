/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:54
	filename: 	\QCore\Library\combinator\seq.cpp
	file path:	\QCore\Library\combinator
	file base:	seq
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_SEQ_CPP_
#define _QLANGUAGE_LIBRARY_COMBINATOR_SEQ_CPP_

#include "seq.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename I, typename O, typename IOO, typename E>
    bool CombinatorSeq<I, O, IOO, E>::parse(const I& input, O& output)
    {
        O result1, result2;
        if (first && first->parse(input, result1))
        {
            if (second && second->parse(Combinator<I, O, IOO, E>::input(result1), result2))
            {
                output = result1 + result2;
                return true;
            }
        }
        return false;
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
