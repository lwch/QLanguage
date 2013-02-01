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
    Rule_Type a('a', context), b('b', context), d('d', context);
    Rule_Type result = (a - d) + +b;

#ifdef _DEBUG
    result.print();
#endif
}
