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
    fs << 'a' << " for test.";
    TEST_ASSERT(fs.size() + fs.cache_size() != 11, "file size and cache size error!");
    fs.flush();
    TEST_ASSERT(fs.size() != 11, "file size error!");
}
