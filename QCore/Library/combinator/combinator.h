/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   22:53
	filename: 	\QCore\Library\combinator\combinator.h
	file path:	\QCore\Library\combinator
	file base:	combinator
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_COMBINATOR_COMBINATOR_H_
#define _QLANGUAGE_LIBRARY_COMBINATOR_COMBINATOR_H_

#include "../definition.h"
#include "../allocator.h"
#include "../construct.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
namespace combinator
{
#define RELEASE_ACHIEVE \
virtual inline void destruct() { QLanguage::Library::destruct(this, has_destruct(*this)); } \
virtual inline const typename QLanguage::Library::combinator::Combinator<I, O, IOO, E>::size_type objSize()const { return sizeof(*this); }

    template <typename O>
    class CombinatorResult
    {
    public:
        CombinatorResult() : bResult(false) {}
        CombinatorResult(bool bResult, const O& result) : bResult(bResult), result(result) {}
        virtual ~CombinatorResult() {}
    public:
        bool bResult;
        O result;
    };

    class DefaultCombinatorResultSelector
    {
    public:
        template <typename I, typename O>
        inline const I operator()(const CombinatorResult<O>& result)const
        {
            return result.result;
        }
    };

    /************************************************************************/
    /* I: Input Type                                                        */
    /* O: Output Type                                                       */
    /* IOO: InputOfOutput                                                   */
    /* E: End of input                                                      */
    /************************************************************************/
    template <typename I, typename O, typename IOO, typename E>
    class Combinator
    {
        typedef Combinator<I, O, IOO, E> self;
    public:
        Combinator() {}
        virtual ~Combinator() {}

        virtual bool parse(const I& input, O& output)=0;

        virtual void destruct()=0;

        typedef size_t size_type;
        virtual const size_type objSize()const=0;
    protected:
        IOO input;
        E   end;
    };
}
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
