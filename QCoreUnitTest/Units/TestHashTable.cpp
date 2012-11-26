#include "TestHashTable.h"

#include "../../QCore/Library/function.h"

TEST_CASE(TestHashTable)
{
    hashtable<int, int, identity<int> > ht;
}