/********************************************************************
	created:	2013/04/08
	created:	8:4:2013   17:09
	filename: 	\QCoreUnitTest\Units\TestStream.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestStream
	file ext:	cpp
	author:		lwch

	purpose:
*********************************************************************/
#include "TestStream.h"

namespace QLanguage
{
    namespace UnitTest
    {
        TEST_CASE(TestStream)
        {
        #ifdef MSVC
            fstream fs("C:\\test.txt", fstream::in | fstream::out);
            TEST_ASSERT(fs.size() != 0, "file size error!");
            fs << 'a' << " for test." << endl;
            TEST_ASSERT(fs.size() + fs.write_cache_size() != 13, "file size and cache size error!");
            fs.flush();
            TEST_ASSERT(fs.size() != 13, "file size error!");

            bool b = false;
            short s = 1;
            ushort us = 2;
            int i = 3;
            uint ui = 4;
            long l = 5;
            ulong ul = 6;
            llong ll = 7;
            ullong ull = 8;
            fs << b << s << us << i << ui << l << ul << ll << ull;
            TEST_ASSERT(fs.size() + fs.write_cache_size() != 22, "file size and cache size error!");
            fs.flush();
            TEST_ASSERT(fs.size() != 22, "file size error!");
            fs.seek(-9, fstream::current);
            fs >> s;
        #else
            int i;
        #endif
            cout << 100 << endl;
            cout << "please input 100:" << endl;
            cin >> i;
            TEST_ASSERT(i != 100, "input number is not 100");
        }
    }
}