/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:54
	filename: 	\QCore\Library\combinator\while.cpp
	file path:	\QCore\Library\combinator
	file base:	while
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_WHILE_CPP_
#define _QLANGUAGE_LIBRARY_COMBINATOR_WHILE_CPP_

#include "while.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename I, typename O, typename IOO, typename E>
    bool CombinatorWhile<I, O, IOO, E>::parse(const I& input, O& output)
    {
        if (!pCombinator) return false;
        if (size == -1)
        {
            O result;
            while (true)
            {
                if (!end(input) && pCombinator->parse(input, result))
                {
                    output = output + result;
                    input = input(result);
                }
                else
                {
                    return true;
                }
            }
            return true;
        }
        else
        {
            O result;
            for (size_type i = 0; i < size; ++i)
            {
                if (!end(input) && pCombinator->parse(input, result))
                {
                    output = output + result;
                    input = input(result);
                }
                else
                {
                    return true;
                }
            }
            return true;
        }
        return false;
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif