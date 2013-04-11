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

TEST_CASE(TestStream)
{
#ifdef WIN32
    fstream fs("C:\\test.txt", fstream::in | fstream::out);
#else
    fstream fs("/opt/test.txt", fstream::in | fstream::out);
#endif
    TEST_ASSERT(fs.size() != 0, "file size error!");
    fs << 'a' << " for test." << endl;
#ifdef WIN32
    TEST_ASSERT(fs.size() + fs.write_cache_size() != 13, "file size and cache size error!");
#else
    TEST_ASSERT(fs.size() + fs.write_cache_size() != 12, "file size and cache size error!");
#endif
    fs.flush();
#ifdef WIN32
    TEST_ASSERT(fs.size() != 13, "file size error!");
#else
    TEST_ASSERT(fs.size() != 13, "file size error!");
#endif

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
#ifdef WIN32
    TEST_ASSERT(fs.size() + fs.write_cache_size() != 22, "file size and cache size error!");
#else
    TEST_ASSERT(fs.size() + fs.write_cache_size() != 21, "file size and cache size error!");
#endif
    fs.flush();
#ifdef WIN32
    TEST_ASSERT(fs.size() != 22, "file size error!");
#else
    TEST_ASSERT(fs.size() != 21, "file size error!");
#endif
    fs.seek(-9, fstream::current);
    fs >> s;
}
