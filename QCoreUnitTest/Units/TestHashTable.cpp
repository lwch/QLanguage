/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:38
	filename: 	\QCoreUnitTest\Units\TestHashTable.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestHashTable
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/

#include "TestHashTable.h"

#include "../../QCore/Library/function.h"

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void test_hashtable()
        {
            hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > > ht;
            typename hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > >::iterator i = ht.insert_equal((T)1);
            TEST_ASSERT(i == ht.end(), "invalid iterator!");
            TEST_ASSERT(ht.find((T)1) == ht.end(), "invalid find value!");
            typename hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > >::iterator j = ht.insert_equal((T)1);
            TEST_ASSERT(i == j, "invalid insert same value!");

            pair<typename hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > >::iterator, bool> k = ht.insert_unique((T)2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(ht.find((T)2) == ht.end(), "invalid find value!");
            pair<typename hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > >::iterator, bool> l = ht.insert_unique((T)1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(ht.size() != 3, "invalid tree size!");
            for (int i = 0; i < 10; ++i)
            {
                ht.insert_equal((T)i);
                switch (i)
                {
                case 1:
                    TEST_ASSERT(ht.count((T)i) != 3, "invalid tree node count with %d!", i);
                    break;
                case 2:
                    TEST_ASSERT(ht.count((T)i) != 2, "invalid tree node count with %d!", i);
                    break;
                default:
                    TEST_ASSERT(ht.count((T)i) != 1, "invalid tree node count with %d!", i);
                    break;
                }
            }
            hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > > ht1 = ht;
            hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > > ht2;
            ht2 = ht1;

            ht.erase(ht.begin());
            TEST_ASSERT(ht.size() != 12, "invalid tree size!");
            ht.erase((T)1);
            TEST_ASSERT(ht.size() != 11, "invalid tree size!");
            ht.erase(ht.begin(), ++++++ht.begin());
            TEST_ASSERT(ht.size() != 8, "invalid tree size!");
            ht.erase(ht.begin(), ht.end());
            TEST_ASSERT(ht.size(), "tree is not empty!");
            TEST_ASSERT(!ht.empty(), "tree is not empty!");
            cout << "test finished with type: " << ht.begin()->type2String() << endl;
        }

        template <typename T>
        void speed_hashtable()
        {
            hashtable<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> >, identity<SmallObject<T> > > hashtable;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                hashtable.insert_equal((T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                hashtable.find((T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            cout << "speed test finished with type: " << hashtable.begin()->type2String() << endl;
        }

        TEST_CASE(TestHashTable_Int)
        {
            hashtable<int, int, identity<int>, identity<int> > ht;
            hashtable<int, int, identity<int>, identity<int> >::iterator i = ht.insert_equal(1);
            TEST_ASSERT(i == ht.end(), "invalid iterator!");
            TEST_ASSERT(ht.find(1) == ht.end(), "invalid find value!");
            hashtable<int, int, identity<int>, identity<int> >::iterator j = ht.insert_equal(1);
            TEST_ASSERT(i == j, "invalid insert same value!");

            pair<hashtable<int, int, identity<int>, identity<int> >::iterator, bool> k = ht.insert_unique(2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(ht.find(2) == ht.end(), "invalid find value!");
            pair<hashtable<int, int, identity<int>, identity<int> >::iterator, bool> l = ht.insert_unique(1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(ht.size() != 3, "invalid tree size!");
            for (int i = 0; i < 10; ++i)
            {
                ht.insert_equal(i);
                switch (i)
                {
                case 1:
                    TEST_ASSERT(ht.count(i) != 3, "invalid tree node count with %d!", i);
                    break;
                case 2:
                    TEST_ASSERT(ht.count(i) != 2, "invalid tree node count with %d!", i);
                    break;
                default:
                    TEST_ASSERT(ht.count(i) != 1, "invalid tree node count with %d!", i);
                    break;
                }
            }
            hashtable<int, int, identity<int>, identity<int> > ht1 = ht;
            hashtable<int, int, identity<int>, identity<int> > ht2;
            ht2 = ht1;

            ht.erase(ht.begin());
            TEST_ASSERT(ht.size() != 12, "invalid tree size!");
            ht.erase(1);
            TEST_ASSERT(ht.size() != 11, "invalid tree size!");
            ht.erase(ht.begin(), ++++++ht.begin());
            TEST_ASSERT(ht.size() != 8, "invalid tree size!");
            ht.erase(ht.begin(), ht.end());
            TEST_ASSERT(ht.size(), "tree is not empty!");
            TEST_ASSERT(!ht.empty(), "tree is not empty!");
        }

        TEST_CASE(TestHashTable_SmallObject)
        {
            test_hashtable<char>();
            test_hashtable<short>();
            test_hashtable<int>();
            test_hashtable<long>();
            test_hashtable<uchar>();
            test_hashtable<ushort>();
            test_hashtable<uint>();
            test_hashtable<ulong>();
            test_hashtable<void*>();
        }

        TEST_CASE(TestHashTable_Speed_Int)
        {
            hashtable<int, int, identity<int>, identity<int> > hashtable;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                hashtable.insert_equal(rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                hashtable.find(rand());
            }
            SHOW_TIME_COST_SECONDS;
        }

        TEST_CASE(TestHashTable_Speed_SmallObject)
        {
            //speed_hashtable<char>(); // 可能堆积在某些桶上导致内存不足
            speed_hashtable<short>();
            speed_hashtable<int>();
            speed_hashtable<long>();
            //speed_hashtable<uchar>(); // 可能堆积在某些桶上导致内存不足
            speed_hashtable<ushort>();
            speed_hashtable<uint>();
            speed_hashtable<ulong>();
            speed_hashtable<void*>();
        }
    }
}