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
    hashtable.insert_equal(10);
    pair<hashtable_type::iterator, bool> result = hashtable.insert_unique(10);
    TEST_ASSERT(result.second, "invalid insert unique value with different value!");
}