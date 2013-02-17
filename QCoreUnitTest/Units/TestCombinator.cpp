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

    inline TestCombinatorResult& operator+=(const TestCombinatorResult& x)
    {
        return *this;
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

typedef CombinatorRule<string, TestCombinatorResult, TestCombinatorResultSelector, emptystring> CombinatorRule_Type;

TEST_CASE(TestCombinator, true)
{
    CombinatorRule_Type a, b, c, d, e;
    CombinatorRule_Type f = a + b;
    CombinatorRule_Type g = c | f;
    a = d + e;
    TestCombinatorResult result;
    g.parse("test", result);
}
