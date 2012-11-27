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