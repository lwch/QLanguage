/********************************************************************
	created:	2012/12/09
	created:	9:12:2012   20:48
	filename: 	\QCore\Library\combinator\ref.cpp
	file path:	\QCore\Library\combinator
	file base:	ref
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_REF_CPP_
#define _QLANGUAGE_LIBRARY_COMBINATOR_REF_CPP_

#include "ref.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename I, typename O, typename IOO, typename E>
    bool CombinatorRef<I, O, IOO, E>::parse(const I& input, O& output)
    {
        return ref.parse(input, output);
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif