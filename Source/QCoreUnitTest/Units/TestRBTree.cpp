/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:40
	filename: 	\QCoreUnitTest\Units\TestRBTree.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestRBTree
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <stdlib.h>

#include "TestRBTree.h"

namespace QLanguage
{
    namespace UnitTest
    {
        template <typename T>
        void test_rbtree()
        {
            rbtree<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> > > tree;
            typename rbtree<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> > >::iterator i = tree.insert_equal((T)1);
            TEST_ASSERT(i == tree.end(), "invalid iterator!");
            TEST_ASSERT(tree.find((T)1) == tree.end(), "invalid find value!");
            typename rbtree<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> > >::iterator j = tree.insert_equal((T)1);
            TEST_ASSERT(i == j, "invalid insert same value!");

            pair<typename rbtree<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> > >::iterator, bool> k = tree.insert_unique((T)2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(tree.find((T)2) == tree.end(), "invalid find value!");
            pair<typename rbtree<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> > >::iterator, bool> l = tree.insert_unique((T)1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(tree.minimum() != tree.begin(), "invalid minimum value with tree begin!");
            TEST_ASSERT(tree.maximum() == tree.end(), "invalid maximum value with tree end!");

            TEST_ASSERT(tree.size() != 3, "invalid tree size!");
            for (int i = 0; i < 10; ++i)
            {
                tree.insert_equal((T)i);
                switch (i)
                {
                case 1:
                    TEST_ASSERT(tree.count((T)i) != 3, "invalid tree node count with %d!", i);
                    break;
                case 2:
                    TEST_ASSERT(tree.count((T)i) != 2, "invalid tree node count with %d!", i);
                    break;
                default:
                    TEST_ASSERT(tree.count((T)i) != 1, "invalid tree node count with %d!", i);
                    break;
                }
            }
            TEST_ASSERT(*tree.minimum() != 0, "invalid minimum of tree: %d", tree.minimum()->value);
            TEST_ASSERT(*tree.maximum() != (T)9, "invalid maximum of tree: %d", tree.maximum()->value);
            TEST_ASSERT(tree.size() != 13, "invalid tree size!");
            tree.erase(tree.begin());
            TEST_ASSERT(*tree.minimum() != (T)1, "invalid minimum of tree: %d", tree.minimum()->value);
            TEST_ASSERT(*tree.maximum() != (T)9, "invalid maximum of tree: %d", tree.maximum()->value);
            TEST_ASSERT(tree.size() != 12, "invalid tree size!");
            tree.erase((T)1);
            TEST_ASSERT(*tree.minimum() != (T)1, "invalid minimum of tree: %d", tree.minimum()->value);
            TEST_ASSERT(*tree.maximum() != (T)9, "invalid maximum of tree: %d", tree.maximum()->value);
            TEST_ASSERT(tree.size() != 11, "invalid tree size!");
            tree.erase(tree.begin(), ++++++tree.begin());
            TEST_ASSERT(*tree.minimum() != (T)2, "invalid minimum of tree: %d", tree.minimum()->value);
            TEST_ASSERT(*tree.maximum() != (T)9, "invalid maximum of tree: %d", tree.maximum()->value);
            TEST_ASSERT(tree.size() != 8, "invalid tree size!");
            tree.erase(tree.begin(), tree.end());
            TEST_ASSERT(tree.minimum() != tree.end(), "invalid minimum iterator of tree!");
            TEST_ASSERT(tree.maximum() != tree.end(), "invalid maximum iterator of tree!");
            TEST_ASSERT(tree.size(), "tree is not empty!");
            TEST_ASSERT(!tree.empty(), "tree is not empty!");

            cout << "test finished with type: " << tree.begin()->type2String() << endl;
        }

        template <typename T>
        void speed_rbtree()
        {
            rbtree<SmallObject<T>, SmallObject<T>, identity<SmallObject<T> > > tree;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                tree.insert_equal((T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                tree.find((T)rand());
            }
            SHOW_TIME_COST_SECONDS;

            cout << "speed test finished with type: " << tree.begin()->type2String() << endl;
        }

        TEST_CASE(TestRBTree_Int)
        {
            rbtree<int, int, identity<int> > tree;
            rbtree<int, int, identity<int> >::iterator i = tree.insert_equal(1);
            TEST_ASSERT(i == tree.end(), "invalid iterator!");
            TEST_ASSERT(tree.find(1) == tree.end(), "invalid find value!");
            rbtree<int, int, identity<int> >::iterator j = tree.insert_equal(1);
            TEST_ASSERT(i == j, "invalid insert same value!");

            pair<rbtree<int, int, identity<int> >::iterator, bool> k = tree.insert_unique(2);
            TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
            TEST_ASSERT(tree.find(2) == tree.end(), "invalid find value!");
            pair<rbtree<int, int, identity<int> >::iterator, bool> l = tree.insert_unique(1);
            TEST_ASSERT(l.second, "invalid insert unique value with same value!");

            TEST_ASSERT(tree.minimum() != tree.begin(), "invalid minimum value with tree begin!");
            TEST_ASSERT(tree.maximum() == tree.end(), "invalid maximum value with tree end!");

            TEST_ASSERT(tree.size() != 3, "invalid tree size!");
            for (int i = 0; i < 10; ++i)
            {
                tree.insert_equal(i);
                switch (i)
                {
                case 1:
                    TEST_ASSERT(tree.count(i) != 3, "invalid tree node count with %d!", i);
                    break;
                case 2:
                    TEST_ASSERT(tree.count(i) != 2, "invalid tree node count with %d!", i);
                    break;
                default:
                    TEST_ASSERT(tree.count(i) != 1, "invalid tree node count with %d!", i);
                    break;
                }
            }
            TEST_ASSERT(*tree.minimum() != 0, "invalid minimum of tree: %d", *tree.minimum());
            TEST_ASSERT(*tree.maximum() != 9, "invalid maximum of tree: %d", *tree.maximum());
            TEST_ASSERT(tree.size() != 13, "invalid tree size!");
            tree.erase(tree.begin());
            TEST_ASSERT(*tree.minimum() != 1, "invalid minimum of tree: %d", *tree.minimum());
            TEST_ASSERT(*tree.maximum() != 9, "invalid maximum of tree: %d", *tree.maximum());
            TEST_ASSERT(tree.size() != 12, "invalid tree size!");
            tree.erase(1);
            TEST_ASSERT(*tree.minimum() != 1, "invalid minimum of tree: %d", *tree.minimum());
            TEST_ASSERT(*tree.maximum() != 9, "invalid maximum of tree: %d", *tree.maximum());
            TEST_ASSERT(tree.size() != 11, "invalid tree size!");
            tree.erase(tree.begin(), ++++++tree.begin());
            TEST_ASSERT(*tree.minimum() != 2, "invalid minimum of tree: %d", *tree.minimum());
            TEST_ASSERT(*tree.maximum() != 9, "invalid maximum of tree: %d", *tree.maximum());
            TEST_ASSERT(tree.size() != 8, "invalid tree size!");
            tree.erase(tree.begin(), tree.end());
            TEST_ASSERT(tree.minimum() != tree.end(), "invalid minimum iterator of tree!");
            TEST_ASSERT(tree.maximum() != tree.end(), "invalid maximum iterator of tree!");
            TEST_ASSERT(tree.size(), "tree is not empty!");
            TEST_ASSERT(!tree.empty(), "tree is not empty!");
        }

        TEST_CASE(TestRBTree_SmallObject)
        {
            test_rbtree<char>();
            test_rbtree<short>();
            test_rbtree<int>();
            test_rbtree<long>();
            test_rbtree<uchar>();
            test_rbtree<ushort>();
            test_rbtree<uint>();
            test_rbtree<ulong>();
            test_rbtree<void*>();
        }

        TEST_CASE(TestRBTree_Speed_Int)
        {
            rbtree<int, int, identity<int> > tree;

            srand(clock());

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                tree.insert_equal(rand());
            }
            SHOW_TIME_COST_SECONDS;

            TIME_START;
            for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
            {
                tree.find(rand());
            }
            SHOW_TIME_COST_SECONDS;
        }

        TEST_CASE(TestRBTree_Speed_SmallObject)
        {
            speed_rbtree<char>();
            speed_rbtree<short>();
            speed_rbtree<int>();
            speed_rbtree<long>();
            speed_rbtree<uchar>();
            speed_rbtree<ushort>();
            speed_rbtree<uint>();
            speed_rbtree<ulong>();
            speed_rbtree<void*>();
        }
    }
}