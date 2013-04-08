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
    fstream fs("C:\\test.txt", fstream::in | fstream::out);
    fs << 'a';
}
