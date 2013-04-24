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

typedef hashtable<int, int, identity<int> > hashtable_type;

TEST_CASE(TestHashTable)
{
    hashtable_type hashtable;
    hashtable_type::iterator i = hashtable.insert_equal(1);
    TEST_ASSERT(i == hashtable.end(), "invalid iterator!");
    TEST_ASSERT(hashtable.find(1) == hashtable.end(), "invalid find value!");
    hashtable_type::iterator j = hashtable.insert_equal(1);
    TEST_ASSERT(i == j, "invalid insert same value!");

    pair<hashtable_type::iterator, bool> k = hashtable.insert_unique(2);
    TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
    TEST_ASSERT(hashtable.find(2) == hashtable.end(), "invalid find value!");
    pair<hashtable_type::iterator, bool> l = hashtable.insert_unique(1);
    TEST_ASSERT(l.second, "invalid insert unique value with same value!");

    TEST_ASSERT(hashtable.size() != 3, "invalid tree size!");
    for (int i = 0; i < 10; ++i)
    {
        hashtable.insert_equal(i);
        switch (i)
        {
        case 1:
            TEST_ASSERT(hashtable.count(i) != 3, "invalid tree node count with %d!", i);
            break;
        case 2:
            TEST_ASSERT(hashtable.count(i) != 2, "invalid tree node count with %d!", i);
            break;
        default:
            TEST_ASSERT(hashtable.count(i) != 1, "invalid tree node count with %d!", i);
            break;
        }
    }
    hashtable.erase(hashtable.begin());
    TEST_ASSERT(hashtable.size() != 12, "invalid tree size!");
    hashtable.erase(1);
    TEST_ASSERT(hashtable.size() != 11, "invalid tree size!");
    hashtable.erase(hashtable.begin(), ++++++hashtable.begin());
    TEST_ASSERT(hashtable.size() != 8, "invalid tree size!");
    hashtable.erase(hashtable.begin(), hashtable.end());
    TEST_ASSERT(hashtable.size(), "tree is not empty!");
    TEST_ASSERT(!hashtable.empty(), "tree is not empty!");
}

TEST_CASE(TestHashTable_Speed)
{
    hashtable_type hashtable;

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