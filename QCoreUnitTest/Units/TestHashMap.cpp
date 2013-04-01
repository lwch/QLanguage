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

#include "TestHashMap.h"

typedef hashmap<int, int> hashmap_type;

TEST_CASE(TestHashMap)
{
    hashmap_type map;
    pair<hashmap_type::iterator, bool> i = map.insert(pair<int, int>(1, 1));
    TEST_ASSERT(!i.second, "insert set error!");
    TEST_ASSERT(i.first == map.end(), "invalid iterator!");
    TEST_ASSERT(map.find(1) == map.end(), "invalid find value!");

    pair<hashmap_type::iterator, bool> k = map.insert(pair<int, int>(2, 2));
    TEST_ASSERT(!k.second, "invalid insert unique value with different value!");
    TEST_ASSERT(map.find(2) == map.end(), "invalid find value!");
    pair<hashmap_type::iterator, bool> l = map.insert(pair<int, int>(1, 1));
    TEST_ASSERT(l.second, "invalid insert unique value with same value!");

    TEST_ASSERT(map.size() != 2, "invalid tree size!");
    for(int i = 0; i < 10; ++i)
    {
        pair<hashmap_type::iterator, bool> r = map.insert(pair<int, int>(i, i));
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

    hashmap_type m;
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

TEST_CASE(TestHashMap_Speed)
{
    hashmap_type map;

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
