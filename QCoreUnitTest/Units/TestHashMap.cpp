/********************************************************************
	created:	2013/04/01
	created:	1:4:2013   14:40
	filename: 	\QCoreUnitTest\Units\TestHashMap.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestHashMap
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <stdlib.h>

#include "TestHashMap.h"

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void test_hashmap()
        {
            hashmap<SmallObject<T>, int> map;
            pair<typename hashmap<SmallObject<T>, int>::iterator, bool> i = map.insert(pair<SmallObject<T>, int>((T)1, 1));
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == map.end(), "invalid iterator!");
            TEST_ASSERT(map.find((T)1) == map.end(), "invalid find value!");

            pair<typename hashmap<SmallObject<T>, int>::iterator, bool> k = map.insert(pair<SmallObject<T>, int>((T)2, 2));
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(map.find((T)2) == map.end(), "invalid find value!");
            pair<typename hashmap<SmallObject<T>, int>::iterator, bool> l = map.insert(pair<SmallObject<T>, int>((T)1, 1));
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(map.size() != 2, "invalid tree size!");
            for(int i = 0; i < 10; ++i)
            {
                pair<typename hashmap<SmallObject<T>, int>::iterator, bool> r = map.insert(pair<SmallObject<T>, int>((T)i, i));
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
                TEST_ASSERT(map[(T)i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(map.size() != 10, "invalid tree size!");
            map.erase(map.begin());
            TEST_ASSERT(map.size() != 9, "invalid tree size!");
            map.erase((T)1);
            TEST_ASSERT(map.size() != 8, "invalid tree size!");
            map.erase(map.begin(), ++++++map.begin());
            TEST_ASSERT(map.size() != 5, "invalid tree size!");
            map.erase(map.begin(), map.end());
            TEST_ASSERT(map.size(), "tree is not empty!");
            TEST_ASSERT(!map.empty(), "tree is not empty!");
            for(int i = 10; i < 20; ++i)
            {
                map[(T)i] = i;
                TEST_ASSERT(map[(T)i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(map.size() != 10, "tree is not empty!");
            TEST_ASSERT(map.empty(), "tree is not empty!");

            hashmap<SmallObject<T>, int> m;
            m[0] = 0;
            m[(T)3] = 3;
            try
            {
                m[(T)2] = 2;
            }
            catch (...)
            {
                PrintError("error to set map key: 2, value: 2!");
            }
            cout << "test finished with type: " << map.begin()->first.type2String() << endl;
        }

        template <typename T>
        void speed_hashmap()
        {
            hashmap<SmallObject<T>, int> map;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                map.insert(pair<SmallObject<T>, int>((T)rand(), i));
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                map.find((T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            cout << "speed test finished with type: " << map.begin()->first.type2String() << endl;
        }

        TEST_CASE(TestHashMap_Int)
        {
            hashmap<int, int> map;
            pair<hashmap<int, int>::iterator, bool> i = map.insert(pair<int, int>(1, 1));
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == map.end(), "invalid iterator!");
            TEST_ASSERT(map.find(1) == map.end(), "invalid find value!");

            pair<hashmap<int, int>::iterator, bool> k = map.insert(pair<int, int>(2, 2));
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(map.find(2) == map.end(), "invalid find value!");
            pair<hashmap<int, int>::iterator, bool> l = map.insert(pair<int, int>(1, 1));
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(map.size() != 2, "invalid tree size!");
            for(int i = 0; i < 10; ++i)
            {
                pair<hashmap<int, int>::iterator, bool> r = map.insert(pair<int, int>(i, i));
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
                TEST_ASSERT(map[i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(map.size() != 10, "invalid tree size!");
            map.erase(map.begin());
            TEST_ASSERT(map.size() != 9, "invalid tree size!");
            map.erase(1);
            TEST_ASSERT(map.size() != 8, "invalid tree size!");
            map.erase(map.begin(), ++++++map.begin());
            TEST_ASSERT(map.size() != 5, "invalid tree size!");
            map.erase(map.begin(), map.end());
            TEST_ASSERT(map.size(), "tree is not empty!");
            TEST_ASSERT(!map.empty(), "tree is not empty!");
            for(int i = 10; i < 20; ++i)
            {
                map[i] = i;
                TEST_ASSERT(map[i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(map.size() != 10, "tree is not empty!");
            TEST_ASSERT(map.empty(), "tree is not empty!");

            hashmap<int, int> m;
            m[0] = 0;
            m[3] = 3;
            try
            {
                m[2] = 2;
            }
            catch (...)
            {
                PrintError("error to set map key: 2, value: 2!");
            }
        }

        TEST_CASE(TestHashMap_SmallObject)
        {
            test_hashmap<char>();
            test_hashmap<short>();
            test_hashmap<int>();
            test_hashmap<long>();
            test_hashmap<uchar>();
            test_hashmap<ushort>();
            test_hashmap<uint>();
            test_hashmap<ulong>();
            test_hashmap<void*>();
        }

        TEST_CASE(TestHashMap_Speed_Int)
        {
            hashmap<int, int> map;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                map.insert(pair<int, int>(rand(), i));
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                map.find(rand());
            }
            SHOW_TIME_COST_SECONDS;
        }

        TEST_CASE(TestHashMap_Speed_SmallObject)
        {
            //speed_hashmap<char>(); // 可能堆积在某些桶上导致内存不足
            speed_hashmap<short>();
            speed_hashmap<int>();
            speed_hashmap<long>();
            //speed_hashmap<uchar>(); // 可能堆积在某些桶上导致内存不足
            speed_hashmap<ushort>();
            speed_hashmap<uint>();
            speed_hashmap<ulong>();
            speed_hashmap<void*>();
        }
    }
}