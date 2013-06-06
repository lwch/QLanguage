/********************************************************************
	created:	2013/04/01
	created:	1:4:2013   14:39
	filename: 	\QCoreUnitTest\Units\TestHashSet.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestHashSet
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <stdlib.h>

#include "TestHashSet.h"

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void test_hashset()
        {
            hashset<SmallObject<T> > set;
            pair<typename hashset<SmallObject<T> >::iterator, bool> i = set.insert((T)1);
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == set.end(), "invalid iterator!");
            TEST_ASSERT(set.find((T)1) == set.end(), "invalid find value!");

            pair<typename hashset<SmallObject<T> >::iterator, bool> k = set.insert((T)2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(set.find((T)2) == set.end(), "invalid find value!");
            pair<typename hashset<SmallObject<T> >::iterator, bool> l = set.insert((T)1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(*set.minimum() != (T)1, "invalid minimum of hashset: %d!", set.minimum()->value);
            TEST_ASSERT(*set.maximum() != (T)2, "invalid maximum of hashset: %d!", set.maximum()->value);

            TEST_ASSERT(set.size() != 2, "invalid hashset size!");
            for(long i = 0; i < 10; ++i)
            {
                pair<typename hashset<SmallObject<T> >::iterator, bool> r = set.insert((T)i);
                switch(i)
                {
                case 1:
                case 2:
                    TEST_ASSERT(r.second, "insert value successed of value: %d!", i);
                    break;
                default:
                    TEST_ASSERT(!r.second, "insert value faild of value: %d!", i);
                    break;
                }
            }
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum()->value);
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 10, "invalid hashset size!");
            set.erase(set.begin());
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum()->value);
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 9, "invalid hashset size!");
            set.erase((T)1);
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum()->value);
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 8, "invalid hashset size!");
            set.erase(set.begin(), ++++++set.begin());
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum()->value);
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 5, "invalid hashset size!");
            set.erase(set.begin(), set.end());
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum()->value);
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum()->value);
            TEST_ASSERT(set.size(), "hashset is not empty!");
            TEST_ASSERT(!set.empty(), "hashset is not empty!");

            cout << "test finished with type: " << set.begin()->type2String() << endl;
        }

        template <typename T>
        void speed_hashset()
        {
            hashset<SmallObject<T> > set;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                set.insert((T)(long)rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                set.find((T)(long)rand());
            }
            SHOW_TIME_COST_SECONDS;

            cout << "speed test finished with type: " << set.begin()->type2String() << endl;
        }

        TEST_CASE(TestHashSet_Int)
        {
            hashset<int> set;
            pair<hashset<int>::iterator, bool> i = set.insert(1);
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == set.end(), "invalid iterator!");
            TEST_ASSERT(set.find(1) == set.end(), "invalid find value!");

            pair<hashset<int>::iterator, bool> k = set.insert(2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(set.find(2) == set.end(), "invalid find value!");
            pair<hashset<int>::iterator, bool> l = set.insert(1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(*set.minimum() != 1, "invalid minimum of hashset: %d!", *set.minimum());
            TEST_ASSERT(*set.maximum() != 2, "invalid maximum of hashset: %d!", *set.maximum());

            TEST_ASSERT(set.size() != 2, "invalid hashset size!");
            for(int i = 0; i < 10; ++i)
            {
                pair<hashset<int>::iterator, bool> r = set.insert(i);
                switch(i)
                {
                case 1:
                case 2:
                    TEST_ASSERT(r.second, "insert value successed of value: %d!", i);
                    break;
                default:
                    TEST_ASSERT(!r.second, "insert value faild of value: %d!", i);
                    break;
                }
            }
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum());
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum());
            TEST_ASSERT(set.size() != 10, "invalid hashset size!");
            set.erase(set.begin());
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum());
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum());
            TEST_ASSERT(set.size() != 9, "invalid hashset size!");
            set.erase(1);
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum());
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum());
            TEST_ASSERT(set.size() != 8, "invalid hashset size!");
            set.erase(set.begin(), ++++++set.begin());
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum());
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum());
            TEST_ASSERT(set.size() != 5, "invalid hashset size!");
            set.erase(set.begin(), set.end());
            TEST_ASSERT(!checkMin(set, *set.minimum()), "invalid minimum of hashset: %d", set.minimum());
            TEST_ASSERT(!checkMax(set, *set.maximum()), "invalid maximum of hashset: %d", set.maximum());
            TEST_ASSERT(set.size(), "hashset is not empty!");
            TEST_ASSERT(!set.empty(), "hashset is not empty!");
        }

        TEST_CASE(TestHashSet_SmallObject)
        {
            //test_hashset<char>(); // 可能堆积在某些桶上导致内存不足
            test_hashset<short>();
            test_hashset<int>();
            test_hashset<long>();
            //test_hashset<uchar>(); // 可能堆积在某些桶上导致内存不足
            test_hashset<ushort>();
            test_hashset<uint>();
            test_hashset<ulong>();
            test_hashset<void*>();
        }

        TEST_CASE(TestHashSet_Speed_Int)
        {
            hashset<int> set;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                set.insert(rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                set.find(rand());
            }
            SHOW_TIME_COST_SECONDS;
        }

        TEST_CASE(TestHashSet_Speed_SmallOjbect)
        {
            speed_hashset<char>();
            speed_hashset<short>();
            speed_hashset<int>();
            speed_hashset<long>();
            speed_hashset<uchar>();
            speed_hashset<ushort>();
            speed_hashset<uint>();
            speed_hashset<ulong>();
            speed_hashset<void*>();
        }
    }
}
