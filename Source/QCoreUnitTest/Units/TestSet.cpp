/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:40
	filename: 	\QCoreUnitTest\Units\TestSet.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestSet
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <stdlib.h>

#include "TestSet.h"

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void test_set()
        {
            set<SmallObject<T> > s;
            pair<typename set<SmallObject<T> >::iterator, bool> i = s.insert((T)1);
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == s.end(), "invalid iterator!");
            TEST_ASSERT(s.find((T)1) == s.end(), "invalid find value!");

            pair<typename set<SmallObject<T> >::iterator, bool> k = s.insert((T)2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(s.find((T)2) == s.end(), "invalid find value!");
            pair<typename set<SmallObject<T> >::iterator, bool> l = s.insert((T)1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(s.minimum() != s.begin(), "invalid minimum value with tree begin!");
            TEST_ASSERT(s.maximum() == s.end(), "invalid maximum value with tree end!");

            TEST_ASSERT(s.size() != 2, "invalid tree size!");
            for(int i = 0; i < 10; ++i)
            {
                pair<typename set<SmallObject<T> >::iterator, bool> r = s.insert((T)i);
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
            TEST_ASSERT(*s.minimum() != 0, "invalid minimum of tree: %d", s.minimum()->value);
            TEST_ASSERT(*s.maximum() != (T)9, "invalid maximum of tree: %d", s.maximum()->value);
            TEST_ASSERT(s.size() != 10, "invalid tree size!");
            s.erase(s.begin());
            TEST_ASSERT(*s.minimum() != (T)1, "invalid minimum of tree: %d", s.minimum()->value);
            TEST_ASSERT(*s.maximum() != (T)9, "invalid maximum of tree: %d", s.maximum()->value);
            TEST_ASSERT(s.size() != 9, "invalid tree size!");
            s.erase((T)1);
            TEST_ASSERT(*s.minimum() != (T)2, "invalid minimum of tree: %d", s.minimum()->value);
            TEST_ASSERT(*s.maximum() != (T)9, "invalid maximum of tree: %d", s.maximum()->value);
            TEST_ASSERT(s.size() != 8, "invalid tree size!");
            s.erase(s.begin(), ++++++s.begin());
            TEST_ASSERT(*s.minimum() != (T)5, "invalid minimum of tree: %d", s.minimum()->value);
            TEST_ASSERT(*s.maximum() != (T)9, "invalid maximum of tree: %d", s.maximum()->value);
            TEST_ASSERT(s.size() != 5, "invalid tree size!");
            s.erase(s.begin(), s.end());
            TEST_ASSERT(s.minimum() != s.end(), "invalid minimum iterator of tree!");
            TEST_ASSERT(s.maximum() != s.end(), "invalid maximum iterator of tree!");
            TEST_ASSERT(s.size(), "tree is not empty!");
            TEST_ASSERT(!s.empty(), "tree is not empty!");

            cout << "test finished with type: " << s.begin()->type2String() << endl;
        }

        template <typename T>
        void speed_set()
        {
            set<SmallObject<T> > set;

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

        TEST_CASE(TestSet_Int)
        {
            set<int> s;
            pair<set<int>::iterator, bool> i = s.insert(1);
            TEST_ASSERT(!i.second, "insert set error!");
            TEST_ASSERT(i.first == s.end(), "invalid iterator!");
            TEST_ASSERT(s.find(1) == s.end(), "invalid find value!");

            pair<set<int>::iterator, bool> k = s.insert(2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(s.find(2) == s.end(), "invalid find value!");
            pair<set<int>::iterator, bool> l = s.insert(1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(s.minimum() != s.begin(), "invalid minimum value with tree begin!");
            TEST_ASSERT(s.maximum() == s.end(), "invalid maximum value with tree end!");

            TEST_ASSERT(s.size() != 2, "invalid tree size!");
            for(int i = 0; i < 10; ++i)
            {
                pair<set<int>::iterator, bool> r = s.insert(i);
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
            TEST_ASSERT(*s.minimum() != 0, "invalid minimum of tree: %d", *s.minimum());
            TEST_ASSERT(*s.maximum() != 9, "invalid maximum of tree: %d", *s.maximum());
            TEST_ASSERT(s.size() != 10, "invalid tree size!");
            s.erase(s.begin());
            TEST_ASSERT(*s.minimum() != 1, "invalid minimum of tree: %d", *s.minimum());
            TEST_ASSERT(*s.maximum() != 9, "invalid maximum of tree: %d", *s.maximum());
            TEST_ASSERT(s.size() != 9, "invalid tree size!");
            s.erase(1);
            TEST_ASSERT(*s.minimum() != 2, "invalid minimum of tree: %d", *s.minimum());
            TEST_ASSERT(*s.maximum() != 9, "invalid maximum of tree: %d", *s.maximum());
            TEST_ASSERT(s.size() != 8, "invalid tree size!");
            s.erase(s.begin(), ++++++s.begin());
            TEST_ASSERT(*s.minimum() != 5, "invalid minimum of tree: %d", *s.minimum());
            TEST_ASSERT(*s.maximum() != 9, "invalid maximum of tree: %d", *s.maximum());
            TEST_ASSERT(s.size() != 5, "invalid tree size!");
            s.erase(s.begin(), s.end());
            TEST_ASSERT(s.minimum() != s.end(), "invalid minimum iterator of tree!");
            TEST_ASSERT(s.maximum() != s.end(), "invalid maximum iterator of tree!");
            TEST_ASSERT(s.size(), "tree is not empty!");
            TEST_ASSERT(!s.empty(), "tree is not empty!");
        }

        TEST_CASE(TestSet_SmallObject)
        {
            test_set<char>();
            test_set<short>();
            test_set<int>();
            test_set<long>();
            test_set<uchar>();
            test_set<ushort>();
            test_set<uint>();
            test_set<ulong>();
            test_set<void*>();
        }

        TEST_CASE(TestSet_Speed_Int)
        {
            set<int> set;

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

        TEST_CASE(TestSet_Speed_SmallObject)
        {
            speed_set<char>();
            speed_set<short>();
            speed_set<int>();
            speed_set<long>();
            speed_set<uchar>();
            speed_set<ushort>();
            speed_set<uint>();
            speed_set<ulong>();
            speed_set<void*>();
        }
    }
}