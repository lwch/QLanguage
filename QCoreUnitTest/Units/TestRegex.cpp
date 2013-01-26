/********************************************************************
	created:	2013/01/20
	created:	20:1:2013   17:55
	filename: 	\QCoreUnitTest\Units\TestRegex.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestRegex
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../../QCore/Library/function.h"
#include "TestRegex.h"

typedef regex::CombinatorNFAContext<char, string> NFAContext;

struct AlwaysFalse : public unary_function<NFAContext, bool>
{
    inline const bool operator()(const NFAContext&)const
    {
        return false;
    }
};

typedef regex::CombinatorCh<char, NFAContext, NFAContext, identity<NFAContext>, AlwaysFalse> ch;
typedef regex::CombinatorRule<NFAContext, NFAContext, identity<NFAContext>, AlwaysFalse> Rule_Type;

TEST_CASE(TestRegex)
{
    ch ch_a('a'), ch_b('b');
    Rule_Type a = ch_a, b = ch_b;
    Rule_Type c = a + b;

    NFAContext nfaContext;
    TEST_ASSERT(!c.parse(nfaContext, nfaContext), "Failed to pase!");
    nfaContext.clear();
}
