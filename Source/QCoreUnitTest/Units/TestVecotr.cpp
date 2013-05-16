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

template <typename T>
inline void speed()
{
    vector<SmallObject> a;

    srand(clock());

    TIME_START;
    for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
    {
        a.push_back((T)(rand() % std::numeric_limits<T>::max()));
    }
    SHOW_TIME_COST_SECONDS;

    TIME_START;
    for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
    {
        find(a.begin(), a.end(), (T)(rand() % std::numeric_limits<T>::max()));
    }
    SHOW_TIME_COST_SECONDS;
}

// template <>
// inline void speed<void*>()
// {
//     vector<SmallObject> a;
// 
//     srand(clock());
// 
//     TIME_START;
//     for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
//     {
//         a.push_back((void*)rand());
//     }
//     SHOW_TIME_COST_SECONDS;
// 
//     TIME_START;
//     for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
//     {
//         find(a.begin(), a.end(), (void*)rand());
//     }
//     SHOW_TIME_COST_SECONDS;
// }

TEST_CASE(TestVector_Int)
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

TEST_CASE(TestVector_SmallObject)
{
    SmallObject l[5] = {1, 2, 3, 4, 5};
    vector<SmallObject> a;
    vector<SmallObject> b(10, 1);
    vector<SmallObject> c(10);
    vector<SmallObject> d(b);
    vector<SmallObject> e(l, l + sizeof(l) / sizeof(SmallObject));
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

TEST_CASE(TestVector_Speed_Int)
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

TEST_CASE(TestVector_Speed_SmallObject)
{
    speed<char>();
    speed<short>();
    //speed<void*>();
}
