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
    template <typename I, typename O, typename IOO = DefaultCombinatorResultSelector>
    class CombinatorSeq : public Combinator<I, O, IOO>
    {
        typedef CombinatorNode<I, O, IOO> node_type;
    public:
        CombinatorSeq(const node_type& first, const node_type& second) : first(first), second(second) {}
        virtual ~CombinatorSeq() {}

        virtual bool parse(const I& input, O& output);
    protected:
        node_type first;
        node_type second;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
