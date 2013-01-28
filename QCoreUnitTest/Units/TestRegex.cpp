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
#include "TestRegex.h"

typedef regex::Rule<char, string> Rule_Type;

TEST_CASE(TestRegex)
{
    Rule_Type::Context context;
    Rule_Type a('a', context), b('b', context);
    Rule_Type c = *(a + b) | +a;

#ifdef _DEBUG
    c.print();
#endif
}
