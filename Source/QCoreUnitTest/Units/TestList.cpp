/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:38
	filename: 	\QCoreUnitTest\Units\TestList.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <stdlib.h>

#include "TestList.h"

#include "../../QCore/Library/algo.h"

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void test_list()
        {
            SmallObject<T> l[5] = {(T)1, (T)2, (T)3, (T)4, (T)5};
            list<SmallObject<T> > a;
            list<SmallObject<T> > b(10, (T)1);
            list<SmallObject<T> > c(10);
            list<SmallObject<T> > d(b);
            list<SmallObject<T> > e(l, l + sizeof(l) / sizeof(int));
            for(int i = 0; i < 10; ++i)
            {
                a.push_back((T)i);
                TEST_ASSERT(a.size()  != (list<int>::size_type)(i + 1), "invalid list size: %d!", a.size());
                TEST_ASSERT(a.front() != 0, "invalid list front value: %d!", a.front().value);
                TEST_ASSERT(a.back()  != (T)i, "invalid list back value: %d!", a.back().value);
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
            TEST_ASSERT(a.empty(), "list is not empty!");

            a.insert(++++a.begin(), 0);
            TEST_ASSERT(a[2] != 0, "invalid list value of index 2!");
            TEST_ASSERT(a.size() != 11, "invalid list size: %d!", a.size());

            for(int i = 0; i < 3; ++i)
            {
                a.pop_back();
                TEST_ASSERT(a.size()  != (list<int>::size_type)(11 - i - 1), "invalid list size: %d!", a.size());
                TEST_ASSERT(a.front() != 0, "invalid list front value: %d!", a.front().value);
                TEST_ASSERT(a.back()  != (T)(10 - i - 2), "invalid list back value: %d!", a.back().value);
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
            TEST_ASSERT(a.empty(), "list is not empty!");

            a.erase(++++a.begin());
            TEST_ASSERT(a[2] == 0, "invalid list value of index 2!");
            TEST_ASSERT(a.size() != 7, "invalid list size: %d!", a.size());

            for(int i = 0; i < 3; ++i)
            {
                a.pop_front();
                TEST_ASSERT(a.size()  != (list<int>::size_type)(7 - i - 1), "invalid list size: %d!", a.size());
                TEST_ASSERT(a.front() != (T)(i + 1), "invalid list front value: %d!", a.front().value);
                TEST_ASSERT(a.back()  != (T)6, "invalid list back value: %d!", a.back().value);
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
            cout << "test finished with type: " << a.front().type2String() << endl;
        }

        template <typename T>
        void speed_list()
        {
            list<SmallObject<T> > a;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                a.push_back((T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                find(a.begin(), a.end(), (T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            cout << "speed test finished with type: " << a.front().type2String() << endl;
        }

        TEST_CASE(TestList_Int)
        {
            int l[5] = {1, 2, 3, 4, 5};
            list<int> a;
            list<int> b(10, 1);
            list<int> c(10);
            list<int> d(b);
            list<int> e(l, l + sizeof(l) / sizeof(int));
            for(int i = 0; i < 10; ++i)
            {
                a.push_back(i);
                TEST_ASSERT(a.size()  != (list<int>::size_type)(i + 1), "invalid list size: %d!", a.size());
                TEST_ASSERT(a.front() != 0, "invalid list front value: %d!", a.front());
                TEST_ASSERT(a.back()  != i, "invalid list back value: %d!", a.back());
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
            TEST_ASSERT(a.empty(), "list is not empty!");

            a.insert(++++a.begin(), 0);
            TEST_ASSERT(a[2] != 0, "invalid list value of index 2!");
            TEST_ASSERT(a.size() != 11, "invalid list size: %d!", a.size());

            for(int i = 0; i < 3; ++i)
            {
                a.pop_back();
                TEST_ASSERT(a.size()  != (list<int>::size_type)(11 - i - 1), "invalid list size: %d!", a.size());
                TEST_ASSERT(a.front() != 0, "invalid list front value: %d!", a.front());
                TEST_ASSERT(a.back()  != 10 - i - 2, "invalid list back value: %d!", a.back());
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
            TEST_ASSERT(a.empty(), "list is not empty!");

            a.erase(++++a.begin());
            TEST_ASSERT(a[2] == 0, "invalid list value of index 2!");
            TEST_ASSERT(a.size() != 7, "invalid list size: %d!", a.size());

            for(int i = 0; i < 3; ++i)
            {
                a.pop_front();
                TEST_ASSERT(a.size()  != (list<int>::size_type)(7 - i - 1), "invalid list size: %d!", a.size());
                TEST_ASSERT(a.front() != i + 1, "invalid list front value: %d!", a.front());
                TEST_ASSERT(a.back()  != 6, "invalid list back value: %d!", a.back());
                TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
            }
        }

        TEST_CASE(TestList_SmallObject)
        {
            test_list<char>();
            test_list<short>();
            test_list<int>();
            test_list<long>();
            test_list<uchar>();
            test_list<ushort>();
            test_list<uint>();
            test_list<ulong>();
            test_list<void*>();
        }

        TEST_CASE(TestList_Speed_Int)
        {
            list<int> a;

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

        TEST_CASE(TestList_Speed_SmallObject)
        {
            speed_list<char>();
            speed_list<short>();
            speed_list<int>();
            speed_list<long>();
            speed_list<uchar>();
            speed_list<ushort>();
            speed_list<uint>();
            speed_list<ulong>();
            speed_list<void*>();
        }
    }
}