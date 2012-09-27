#include "TestRBTree.h"
#include <time.h>

typedef rbtree<int, int, identity<int> > tree_type;

#define TEST_SPEED_INSERT_COUNT 10000

TEST_CASE(TestRBTree)
{
    tree_type tree;
    tree_type::iterator i = tree.insert_equal(1);
    TEST_ASSERT(i == tree.end(), "invalid iterator!");
    TEST_ASSERT(tree.find(1) == tree.end(), "invalid find value!");
    tree_type::iterator j = tree.insert_equal(1);
    TEST_ASSERT(i == j, "invalid insert same value!");

    pair<tree_type::iterator, bool> k = tree.insert_unique(2);
    TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
    TEST_ASSERT(tree.find(2) == tree.end(), "invalid find value!");
    pair<tree_type::iterator, bool> l = tree.insert_unique(1);
    TEST_ASSERT(l.second, "invalid insert unique value with same value!");

    TEST_ASSERT(tree.minimum() != tree.begin(), "invalid minimum value with tree begin!");
    TEST_ASSERT(tree.maximum() == tree.end(), "invalid maximum value with tree end!");

    TEST_ASSERT(tree.size() != 3, "invalid tree size!");
    for(int i = 0; i < 10; ++i)
    {
        tree.insert_equal(i); // TOFIX i == 2时root不正确
        switch(i)
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

TEST_CASE(TestRBTree_Speed)
{
    tree_type tree;

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