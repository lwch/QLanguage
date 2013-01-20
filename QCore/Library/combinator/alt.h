/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:54
	filename: 	\QCore\Library\combinator\alt.h
	file path:	\QCore\Library\combinator
	file base:	alt
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_ALT_H_
#define _QLANGUAGE_LIBRARY_COMBINATOR_ALT_H_

#include "combinator.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace combinator
{
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorAlt : public Combinator<I, O, IOO, E>
    {
        typedef Combinator<I, O, IOO, E> combinator_type;
    public:
        CombinatorAlt(combinator_type* first, combinator_type* second) : first(first), second(second) {}
        virtual ~CombinatorAlt() {}

        virtual bool parse(const I& input, O& output)
        {
            O result1, result2;
            if (first && first->parse(input, result1))
            {
                output = result1;
                return this->hook(this);
            }
            if (second && second->parse(input, result2))
            {
                output = result2;
                return this->hook(this);
            }
            return false;
        }

        virtual inline bool hook(const combinator_type* pCombinator) { return true; }

        virtual inline void destruct() { QLanguage::Library::destruct(this, has_destruct(*this)); }
        virtual inline const typename Combinator<I, O, IOO, E>::size_type objSize()const { return sizeof(*this); }
    protected:
        combinator_type* first;
        combinator_type* second;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif