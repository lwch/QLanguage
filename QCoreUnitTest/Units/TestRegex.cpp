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

typedef regex::CombinatorNode<string, TestCombinatorResult, TestCombinatorResultSelector, emptystring> CombinatorRule_Type;

TEST_CASE(TestRegex)
{
    CombinatorRule_Type a, b;
    CombinatorRule_Type c = a + b;
    TestCombinatorResult result;
    c.parse("test", result);
}
