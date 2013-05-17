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
        void test()
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

            TEST_ASSERT(set.minimum() != set.begin(), "invalid minimum value with tree begin!");
            TEST_ASSERT(set.maximum() == set.end(), "invalid maximum value with tree end!");

            TEST_ASSERT(set.size() != 2, "invalid tree size!");
            for(int i = 0; i < 10; ++i)
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
            TEST_ASSERT(*set.minimum() != 0, "invalid minimum of tree: %d", set.minimum()->value);
            TEST_ASSERT(*set.maximum() != (T)9, "invalid maximum of tree: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 10, "invalid tree size!");
            set.erase(set.begin());
            TEST_ASSERT(*set.minimum() != (T)1, "invalid minimum of tree: %d", set.minimum()->value);
            TEST_ASSERT(*set.maximum() != (T)9, "invalid maximum of tree: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 9, "invalid tree size!");
            set.erase((T)1);
            TEST_ASSERT(*set.minimum() != (T)2, "invalid minimum of tree: %d", set.minimum()->value);
            TEST_ASSERT(*set.maximum() != (T)9, "invalid maximum of tree: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 8, "invalid tree size!");
            set.erase(set.begin(), ++++++set.begin());
            TEST_ASSERT(*set.minimum() != (T)5, "invalid minimum of tree: %d", set.minimum()->value);
            TEST_ASSERT(*set.maximum() != (T)9, "invalid maximum of tree: %d", set.maximum()->value);
            TEST_ASSERT(set.size() != 5, "invalid tree size!");
            set.erase(set.begin(), set.end());
            TEST_ASSERT(set.minimum() != set.end(), "invalid minimum iterator of tree!");
            TEST_ASSERT(set.maximum() != set.end(), "invalid maximum iterator of tree!");
            TEST_ASSERT(set.size(), "tree is not empty!");
            TEST_ASSERT(!set.empty(), "tree is not empty!");

            cout << "test finished with type: " << set.begin()->type2String() << endl;
        }

        template <typename T>
        void speed()
        {
            hashset<SmallObject<T> > set;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                set.insert((T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                set.find((T)rand());
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

            TEST_ASSERT(set.minimum() != set.begin(), "invalid minimum value with tree begin!");
            TEST_ASSERT(set.maximum() == set.end(), "invalid maximum value with tree end!");

            TEST_ASSERT(set.size() != 2, "invalid tree size!");
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
            TEST_ASSERT(*set.minimum() != 0, "invalid minimum of tree: %d", *set.minimum());
            TEST_ASSERT(*set.maximum() != 9, "invalid maximum of tree: %d", *set.maximum());
            TEST_ASSERT(set.size() != 10, "invalid tree size!");
            set.erase(set.begin());
            TEST_ASSERT(*set.minimum() != 1, "invalid minimum of tree: %d", *set.minimum());
            TEST_ASSERT(*set.maximum() != 9, "invalid maximum of tree: %d", *set.maximum());
            TEST_ASSERT(set.size() != 9, "invalid tree size!");
            set.erase(1);
            TEST_ASSERT(*set.minimum() != 2, "invalid minimum of tree: %d", *set.minimum());
            TEST_ASSERT(*set.maximum() != 9, "invalid maximum of tree: %d", *set.maximum());
            TEST_ASSERT(set.size() != 8, "invalid tree size!");
            set.erase(set.begin(), ++++++set.begin());
            TEST_ASSERT(*set.minimum() != 5, "invalid minimum of tree: %d", *set.minimum());
            TEST_ASSERT(*set.maximum() != 9, "invalid maximum of tree: %d", *set.maximum());
            TEST_ASSERT(set.size() != 5, "invalid tree size!");
            set.erase(set.begin(), set.end());
            TEST_ASSERT(set.minimum() != set.end(), "invalid minimum iterator of tree!");
            TEST_ASSERT(set.maximum() != set.end(), "invalid maximum iterator of tree!");
            TEST_ASSERT(set.size(), "tree is not empty!");
            TEST_ASSERT(!set.empty(), "tree is not empty!");
        }

        TEST_CASE(TestHashSet_SmallObject)
        {
            test<char>();
            test<short>();
            test<int>();
            test<long>();
            test<uchar>();
            test<ushort>();
            test<uint>();
            test<ulong>();
            test<void*>();
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
            speed<char>();
            speed<short>();
            speed<int>();
            speed<long>();
            speed<uchar>();
            speed<ushort>();
            speed<uint>();
            speed<ulong>();
            speed<void*>();
        }
    }
}