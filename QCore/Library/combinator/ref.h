/********************************************************************
	created:	2012/12/09
	created:	9:12:2012   20:45
	filename: 	\QCore\Library\combinator\ref.h
	file path:	\QCore\Library\combinator
	file base:	ref
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_REF_H_
#define _QLANGUAGE_LIBRARY_COMBINATOR_REF_H_

#include "combinator.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorRef : public Combinator<I, O, IOO, E>
    {
        typedef CombinatorRef<I, O, IOO, E> self;
        typedef Combinator<I, O, IOO, E>* combinator_type;
    public:
        CombinatorRef(combinator_type& ref) : ref(ref) {}
        virtual ~CombinatorRef() {}

        virtual inline bool parse(const I& input, O& output)
        {
            return ref ? ref->parse(input, output) : false;
        }

        virtual inline void destruct() { QLanguage::Library::destruct(this, has_destruct(*this)); }
        virtual inline const typename Combinator<I, O, IOO, E>::size_type objSize()const { return sizeof(*this); }
    protected:
        combinator_type& ref;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif