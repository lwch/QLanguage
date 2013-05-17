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

typedef regex::Rule Rule_Type;

namespace QLanguage
{
    namespace UnitTest
    {
        TEST_CASE(TestRegex)
        {
        //     Rule_Type::Context context;
        //     Rule_Type a('a', &context), b('b', &context), d('d', &context);
        //     Rule_Type result = (a - d).opt() + (+b | !(a + b));
        //     result.buildDFA();
        // 
        // #ifdef _DEBUG
        //     result.printEpsilonNFA();
        //     result.printDFA();
        // #endif

            static size_t iContentSize = 1024 * 1024;
            static char   charMap[67]  = {0};
            for (int i = 0; i < 10; ++i)
            {
                charMap[i] = '0' + i;
            }
            for (int i = 0; i < 26; ++i)
            {
                charMap[10 + i] = 'a' + i;
                charMap[36 + i] = 'A' + i;
            }
            charMap[62] = '_';
            charMap[63] = ' ';
            charMap[64] = '\t';
            charMap[65] = '\r';
            charMap[66] = '\n';

            Rule_Type::Context context;

            Rule_Type _0('0', &context), _9('9', &context), _0x("0x", &context), _0X("0X", &context), point('.', &context);
            Rule_Type digit = +(_0 - _9);
            Rule_Type hex = (_0x | _0X) + digit;
            Rule_Type real = digit.opt() + point + digit;

            Rule_Type _('_', &context), a('a', &context), z('z', &context), A('A', &context), Z('Z', &context);
            Rule_Type letter = +(_ | (_0 - _9) | (a - z) | (A - Z));

            Rule_Type letterSpace(' ', &context), letterTab('\t', &context);
            Rule_Type space = +(letterSpace | letterTab);

            Rule_Type __r('\r', &context), __n('\n', &context), __rn("\r\n", &context);
            Rule_Type enter = +(__r | __n | __rn);

            digit.buildDFA();
            hex.buildDFA();
            real.buildDFA();
            letter.buildDFA();
            space.buildDFA();
            enter.buildDFA();

        #ifdef _DEBUG
            cout << "========== digit start ==========" << endl;
            digit.printEpsilonNFA(cout);
            digit.printDFA(cout);
            cout << "=========== digit end ===========" << endl;

            cout << "========== hex start ==========" << endl;
            hex.printEpsilonNFA(cout);
            hex.printDFA(cout);
            cout << "=========== hex end ===========" << endl;

            cout << "========== real start ==========" << endl;
            real.printEpsilonNFA(cout);
            real.printDFA(cout);
            cout << "=========== real end ===========" << endl;

            cout << "========== letter start ==========" << endl;
            letter.printEpsilonNFA(cout);
            letter.printDFA(cout);
            cout << "=========== letter end ===========" << endl;

            cout << "========== space start ==========" << endl;
            space.printEpsilonNFA(cout);
            space.printDFA(cout);
            cout << "=========== space end ===========" << endl;

            cout << "========== enter start ==========" << endl;
            enter.printEpsilonNFA(cout);
            enter.printDFA(cout);
            cout << "=========== enter end ===========" << endl;
        #endif

            char* szTest = allocator<char>::allocate(iContentSize + 1);
            srand(clock());
            for (size_t i = 0; i < iContentSize; ++i)
            {
                szTest[i] = charMap[rand() % sizeof(charMap)];
            }
            szTest[iContentSize] = 0;

            char *first = szTest, *last = first + iContentSize;
            char *s = NULL;
            size_t size = 0;
            clock_t t = clock();
            while (first != last)
            {
                if (digit.parse(first, last, s, size))       first += size;
                else if (hex.parse(first, last, s, size))    first += size;
                else if (real.parse(first, last, s, size))   first += size;
                else if (letter.parse(first, last, s, size)) first += size;
                else if (space.parse(first, last, s, size))  first += size;
                else if (enter.parse(first, last, s, size))  first += size;
            }
            t = clock() - t;
            allocator<char>::deallocate(szTest, iContentSize);
            cout << string::format("parse data length: %lu bytes", (unsigned long)iContentSize) << endl;
            cout << string::format("use of time: %fs", t / 1000.0f) << endl;
        }
    }
}