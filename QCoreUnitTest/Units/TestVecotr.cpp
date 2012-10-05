#include "TestVector.h"
#include "../../QCore/Library/string.h"
#include <vector>

TEST_CASE(TestVector_Construct)
{
    int l[5] = {1, 2, 3, 4, 5};
    vector<int> a;
    vector<int> b(10, 1);
    vector<int> c(10);
    vector<int> d(b);
    vector<int> e(l, l + sizeof(l) / sizeof(int));
    for(int i = 0; i < 10; ++i)
    {
        a.push_back(i);
        TEST_ASSERT(a.size()  != i + 1, "invalid vector size: %d!", a.size());
        TEST_ASSERT(a.front() != 0, "invalid vector front value: %d!", i);
        TEST_ASSERT(a.back()  != i, "invalid vector back value: %d!", i);
        TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
    }
    TEST_ASSERT(a.empty(), "vector is not empty!");
    //a.insert(++++a.begin(), 0);
}

TEST_CASE(TestVector_Speed)
{
    vector<int> a;

    srand(clock());

    TIME_START;
    for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
    {
        a.push_back(rand());
    }
    SHOW_TIME_COST_SECONDS;

    TIME_START;
    for(int i = 0; i < TEST_SPEED_INSERT_COUNT; ++i)
    {
        find(a.begin(), a.end(), rand());
    }
    SHOW_TIME_COST_SECONDS;
}