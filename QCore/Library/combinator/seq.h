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
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorSeq : public Combinator<I, O, IOO, E>
    {
        typedef Combinator<I, O, IOO, E> combinator_type;
    public:
        CombinatorSeq(combinator_type* first, combinator_type* second) : first(first), second(second) {}
        virtual ~CombinatorSeq() {}

        virtual bool parse(const I& input, O& output)
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

        virtual inline void destruct() { QLanguage::Library::destruct(this, has_destruct(*this)); }
        virtual inline const typename Combinator<I, O, IOO, E>::size_type objSize()const { return sizeof(*this); }
    protected:
        combinator_type* first;
        combinator_type* second;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
