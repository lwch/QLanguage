/********************************************************************
	created:	2012/12/09
	created:	9:12:2012   20:59
	filename: 	\QCoreUnitTest\Units\TestCombinator.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestCombinator
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../../QCore/Library/string.h"
#include "../../QCore/Library/function.h"
#include "TestCombinator.h"

class TestCombinatorResult : public CombinatorResult<string>
{
public:
    TestCombinatorResult operator+(const TestCombinatorResult& result)
    {
        return TestCombinatorResult();
    }
};

class TestCombinatorResultSelector : public DefaultCombinatorResultSelector
{
public:
    const string operator()(const TestCombinatorResult& result)const
    {
        return result.result;
    }
};

typedef CombinatorNode<string, TestCombinatorResult, TestCombinatorResultSelector, emptystring> CombinatorNode_Type;

TEST_CASE(TestCombinator)
{
    CombinatorNode_Type a, b, c;
    CombinatorNode_Type d = a + b;
    CombinatorNode_Type e = c | d;
    TestCombinatorResult result;
    e.parse("test", result);
}
