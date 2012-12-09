/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:54
	filename: 	\QCore\Library\combinator\alt.cpp
	file path:	\QCore\Library\combinator
	file base:	alt
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_ALT_CPP_
#define _QLANGUAGE_LIBRARY_COMBINATOR_ALT_CPP_

#include "alt.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename I, typename O, typename IOO, typename E>
    bool CombinatorAlt<I, O, IOO, E>::parse(const I& input, O& output)
    {
        O result1, result2;
        if (first && first->parse(input, result1))
        {
            output = result1;
            return true;
        }
        if (second && second->parse(input, result2))
        {
            output = result2;
            return true;
        }
        return false;
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
