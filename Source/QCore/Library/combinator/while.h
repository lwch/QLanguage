/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:54
	filename: 	\QCore\Library\combinator\while.h
	file path:	\QCore\Library\combinator
	file base:	while
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_WHILE_H_
#define _QLANGUAGE_LIBRARY_COMBINATOR_WHILE_H_

#include "combinator.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace combinator
{
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorWhile : public Combinator<I, O, IOO, E>
    {
        typedef Combinator<I, O, IOO, E> combinator_type;
        typedef int                      size_type;
    public:
        CombinatorWhile(combinator_type* pCombinator, bool bOnceMore) : pCombinator(pCombinator), bOnceMore(bOnceMore) {}
        virtual ~CombinatorWhile() {}

        virtual bool parse(const I& input, O& output)
        {
            if (!pCombinator) return false;
            if (bOnceMore && (end(input) || !pCombinator->parse(input, output))) return false;
            while (true)
            {
                if (!end(input) && pCombinator->parse(input, output))
                {
                    input = input(output);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        RELEASE_ACHIEVE
    protected:
        combinator_type* pCombinator;
        bool             bOnceMore;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif