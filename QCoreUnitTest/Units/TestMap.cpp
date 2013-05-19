/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:39
	filename: 	\QCoreUnitTest\Units\TestMap.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestMap
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <stdlib.h>

#include "TestMap.h"

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void test_map()
        {
            map<SmallObject<T>, int> m;
            pair<typename map<SmallObject<T>, int>::iterator, bool> i = m.insert(pair<SmallObject<T>, int>((T)1, 1));
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == m.end(), "invalid iterator!");
            TEST_ASSERT(m.find((T)1) == m.end(), "invalid find value!");

            pair<typename map<SmallObject<T>, int>::iterator, bool> k = m.insert(pair<SmallObject<T>, int>((T)2, 2));
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(m.find((T)2) == m.end(), "invalid find value!");
            pair<typename map<SmallObject<T>, int>::iterator, bool> l = m.insert(pair<SmallObject<T>, int>((T)1, 1));
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(m.size() != 2, "invalid tree size!");
            for(int i = 0; i < 10; ++i)
            {
                pair<typename map<SmallObject<T>, int>::iterator, bool> r = m.insert(pair<SmallObject<T>, int>((T)i, i));
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
                TEST_ASSERT(m[(T)i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(m.size() != 10, "invalid tree size!");
            m.erase(m.begin());
            TEST_ASSERT(m.size() != 9, "invalid tree size!");
            m.erase((T)1);
            TEST_ASSERT(m.size() != 8, "invalid tree size!");
            m.erase(m.begin(), ++++++m.begin());
            TEST_ASSERT(m.size() != 5, "invalid tree size!");
            m.erase(m.begin(), m.end());
            TEST_ASSERT(m.size(), "tree is not empty!");
            TEST_ASSERT(!m.empty(), "tree is not empty!");
            for(int i = 10; i < 20; ++i)
            {
                m[(T)i] = i;
                TEST_ASSERT(m[(T)i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(m.size() != 10, "tree is not empty!");
            TEST_ASSERT(m.empty(), "tree is not empty!");

            map<int, int> m1;
            m1[0] = 0;
            m1[3] = 3;
            try
            {
                m1[2] = 2;
            }
            catch (...)
            {
                PrintError("error to set map key: 2, value: 2!");
            }
            cout << "test finished with type: " << m.begin()->first.type2String() << endl;
        }

        template <typename T>
        void speed_map()
        {
            map<SmallObject<T>, int> map;

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

        TEST_CASE(TestMap_Int)
        {
            map<int, int> m;
            pair<map<int, int>::iterator, bool> i = m.insert(pair<int, int>(1, 1));
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == m.end(), "invalid iterator!");
            TEST_ASSERT(m.find(1) == m.end(), "invalid find value!");

            pair<map<int, int>::iterator, bool> k = m.insert(pair<int, int>(2, 2));
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(m.find(2) == m.end(), "invalid find value!");
            pair<map<int, int>::iterator, bool> l = m.insert(pair<int, int>(1, 1));
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(m.size() != 2, "invalid tree size!");
            for(int i = 0; i < 10; ++i)
            {
                pair<map<int, int>::iterator, bool> r = m.insert(pair<int, int>(i, i));
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
                TEST_ASSERT(m[i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(m.size() != 10, "invalid tree size!");
            m.erase(m.begin());
            TEST_ASSERT(m.size() != 9, "invalid tree size!");
            m.erase(1);
            TEST_ASSERT(m.size() != 8, "invalid tree size!");
            m.erase(m.begin(), ++++++m.begin());
            TEST_ASSERT(m.size() != 5, "invalid tree size!");
            m.erase(m.begin(), m.end());
            TEST_ASSERT(m.size(), "tree is not empty!");
            TEST_ASSERT(!m.empty(), "tree is not empty!");
            for(int i = 10; i < 20; ++i)
            {
                m[i] = i;
                TEST_ASSERT(m[i] != i, "invalid value of key: %d!", i);
            }
            TEST_ASSERT(m.size() != 10, "tree is not empty!");
            TEST_ASSERT(m.empty(), "tree is not empty!");

            map<int, int> m1;
            m1[0] = 0;
            m1[3] = 3;
            try
            {
                m1[2] = 2;
            }
            catch (...)
            {
                PrintError("error to set map key: 2, value: 2!");
            }
        }

        TEST_CASE(TestMap_SmallObject)
        {
            test_map<char>();
            test_map<short>();
            test_map<int>();
            test_map<long>();
            test_map<uchar>();
            test_map<ushort>();
            test_map<uint>();
            test_map<ulong>();
            test_map<void*>();
        }

        TEST_CASE(TestMap_Speed_Int)
        {
            map<int, int> map;

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

        TEST_CASE(TestMap_Speed_SmallObject)
        {
            speed_map<char>();
            speed_map<short>();
            speed_map<int>();
            speed_map<long>();
            speed_map<uchar>();
            speed_map<ushort>();
            speed_map<uint>();
            speed_map<ulong>();
            speed_map<void*>();
        }
    }
}