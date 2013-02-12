/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:42
	filename: 	\QCoreUnitTest\Units\TestVecotr.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestVecotr
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <stdlib.h>

#include "TestVector.h"

TEST_CASE(TestVector)
{
    int l[5] = {1, 2, 3, 4, 5};
    vector<int> a;
    vector<int> b(10, 1);
    vector<int> c(10);
    vector<int> d(b);
    vector<int> e(l, l + sizeof(l) / sizeof(int));
    for(int i = 0; i < 10; ++i)
    {
        a.push_back(i);
        TEST_ASSERT(a.size()  != (vector<int>::size_type)(i + 1), "invalid vector size: %d!", a.size());
        TEST_ASSERT(a.front() != 0, "invalid vector front value: %d!", a.front());
        TEST_ASSERT(a.back()  != i, "invalid vector back value: %d!", a.back());
        TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
    }
    TEST_ASSERT(a.empty(), "vector is not empty!");

    a.insert(++++a.begin(), 0);
    TEST_ASSERT(a[2] != 0, "invalid vector value of index 2!");
    TEST_ASSERT(a.size() != 11, "invalid vector size: %d!", a.size());

    for(int i = 0; i < 3; ++i)
    {
        a.pop_back();
        TEST_ASSERT(a.size()  != (vector<int>::size_type)(11 - i - 1), "invalid vector size: %d!", a.size());
        TEST_ASSERT(a.front() != 0, "invalid vector front value: %d!", a.front());
        TEST_ASSERT(a.back()  != 10 - i - 2, "invalid vector back value: %d!", a.back());
        TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
    }
    TEST_ASSERT(a.empty(), "vector is not empty!");

    a.erase(++++a.begin());
    TEST_ASSERT(a[2] == 0, "invalid vector value of index 2!");
    TEST_ASSERT(a.size() != 7, "invalid vector size: %d!", a.size());
}

TEST_CASE(TestVector_Speed)
{
    vector<int> a;

    srand(clock());

    TIME_START;
    for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
    {
        a.push_back(rand());
    }
    SHOW_TIME_COST_SECONDS;

    TIME_START;
    for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
    {
        find(a.begin(), a.end(), rand());
    }
    SHOW_TIME_COST_SECONDS;
}
