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
    template <typename I, typename O, typename IOO, typename E>
    class CombinatorWhile : public Combinator<I, O, IOO, E>
    {
        typedef Combinator<I, O, IOO, E> combinator_type;
        typedef int                      size_type;
    public:
        CombinatorWhile(combinator_type* pCombinator, size_type size) : pCombinator(pCombinator), size(size) {}
        virtual ~CombinatorWhile() {}

        virtual bool parse(const I& input, O& output)
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

        virtual inline void destruct() { QLanguage::Library::destruct(this, has_destruct(*this)); }
        virtual inline const typename Combinator<I, O, IOO, E>::size_type objSize()const { return sizeof(*this); }
    protected:
        combinator_type* pCombinator;
        size_type        size;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif