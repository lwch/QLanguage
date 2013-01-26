/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:53
	filename: 	\QCore\Library\combinator\seq.h
	file path:	\QCore\Library\combinator
	file base:	seq
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_SEQ_H_
#define _QLANGUAGE_LIBRARY_COMBINATOR_SEQ_H_

#include "combinator.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace combinator
{
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorSeq : public Combinator<I, O, IOO, E>
    {
        typedef Combinator<I, O, IOO, E> combinator_type;
    public:
        CombinatorSeq(combinator_type* first, combinator_type* second) : first(first), second(second) {}
        virtual ~CombinatorSeq() {}

        virtual bool parse(const I& input, O& output)
        {
            if (first && first->parse(input, output))
            {
                if (second && second->parse(Combinator<I, O, IOO, E>::input(output), output))
                {
                    return true;
                }
            }
            return false;
        }

        RELEASE_ACHIEVE
    protected:
        combinator_type* first;
        combinator_type* second;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
