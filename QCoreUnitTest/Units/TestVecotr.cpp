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

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void speed_vector()
        {
            vector<SmallObject<T> > a;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                a.push_back((T)(long)rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                find(a.begin(), a.end(), (T)(long)rand());
            }
            SHOW_TIME_COST_SECONDS;

            cout << "speed test finished with type: " << a.front().type2String() << endl;
        }

        template <typename T>
        void test_vector()
        {
            SmallObject<T> l[5] = {(T)1, (T)2, (T)3, (T)4, (T)5};
            vector<SmallObject<T> > a;
            vector<SmallObject<T> > b(10, (T)1);
            vector<SmallObject<T> > c(10);
            vector<SmallObject<T> > d(b);
            vector<SmallObject<T> > e(l, l + sizeof(l) / sizeof(SmallObject<T>));
            for(long i = 0; i < 10; ++i)
            {
                a.push_back((T)i);
                TEST_ASSERT(a.size()  != (vector<int>::size_type)(i + 1), "invalid vector size: %d!", a.size());
                TEST_ASSERT(a.front() != 0, "invalid vector front value: %d!", a.front().value);
                TEST_ASSERT(a.back()  != (T)i, "invalid vector back value: %d!", a.back().value);
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
            TEST_ASSERT(a.empty(), "vector is not empty!");

            a.insert(++++a.begin(), 0);
            TEST_ASSERT(a[2] != 0, "invalid vector value of index 2!");
            TEST_ASSERT(a.size() != 11, "invalid vector size: %d!", a.size());

            for(long i = 0; i < 3; ++i)
            {
                a.pop_back();
                TEST_ASSERT(a.size()  != (vector<int>::size_type)(11 - i - 1), "invalid vector size: %d!", a.size());
                TEST_ASSERT(a.front() != 0, "invalid vector front value: %d!", a.front().value);
                TEST_ASSERT(a.back()  != (T)(10 - i - 2), "invalid vector back value: %d!", a.back().value);
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
            TEST_ASSERT(a.empty(), "vector is not empty!");

            a.erase(++++a.begin());
            TEST_ASSERT(a[2] == 0, "invalid vector value of index 2!");
            TEST_ASSERT(a.size() != 7, "invalid vector size: %d!", a.size());

            cout << "test finished with type: " << a.front().type2String() << endl;
        }

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
            test_vector<char>();
            test_vector<short>();
            test_vector<int>();
            test_vector<long>();
            test_vector<uchar>();
            test_vector<ushort>();
            test_vector<uint>();
            test_vector<ulong>();
            test_vector<void*>();
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
            speed_vector<char>();
            speed_vector<short>();
            speed_vector<int>();
            speed_vector<long>();
            speed_vector<uchar>();
            speed_vector<ushort>();
            speed_vector<uint>();
            speed_vector<ulong>();
            speed_vector<void*>();
        }
    }
}
