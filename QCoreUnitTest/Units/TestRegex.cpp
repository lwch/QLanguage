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

TEST_CASE(TestRegex, true)
{
//     Rule_Type::Context context;
//     Rule_Type a('a', context), b('b', context), d('d', context);
//     Rule_Type result = (a - d).opt() + (+b | !(a + b));
//     result.buildDFA();
// 
// #ifdef _DEBUG
//     result.printEpsilonNFA();
//     result.printDFA();
// #endif

    Rule_Type::Context context;

    Rule_Type _0('0', context), _9('9', context), _0x("0x", context), _0X("0X", context), point('.', context);
    Rule_Type digit = +(_0 - _9);
    Rule_Type hex = (_0x | _0X) + digit;
    Rule_Type real = digit.opt() + point + digit;

    Rule_Type _('_', context), a('a', context), z('z', context), A('A', context), Z('Z', context);
    Rule_Type letter = +(_ | (_0 - _9) | (a - z) | (A - Z));

    Rule_Type letterSpace(' ', context), letterTab('\t', context);
    Rule_Type space = +(letterSpace | letterTab);

    Rule_Type __r('\r', context), __n('\n', context), __rn("\r\n", context);
    Rule_Type enter = +(__r | __n | __rn);

    digit.buildDFA();
    hex.buildDFA();
    real.buildDFA();
    letter.buildDFA();
    space.buildDFA();
    enter.buildDFA();

#ifdef _DEBUG
    printf("========== digit start ==========\n");
    digit.printEpsilonNFA();
    digit.printDFA();
    printf("=========== digit end ===========\n");

    printf("========== hex start ==========\n");
    hex.printEpsilonNFA();
    hex.printDFA();
    printf("=========== hex end ===========\n");

    printf("========== real start ==========\n");
    real.printEpsilonNFA();
    real.printDFA();
    printf("=========== real end ===========\n");

    printf("========== letter start ==========\n");
    letter.printEpsilonNFA();
    letter.printDFA();
    printf("=========== letter end ===========\n");

    printf("========== space start ==========\n");
    space.printEpsilonNFA();
    space.printDFA();
    printf("=========== space end ===========\n");

    printf("========== enter start ==========\n");
    enter.printEpsilonNFA();
    enter.printDFA();
    printf("=========== enter end ===========\n");
#endif
}
