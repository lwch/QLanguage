#include "TestList.h"

#include "../../QCore/Library/algo.h"

TEST_CASE(TestList)
{
    int l[5] = {1, 2, 3, 4, 5};
    list<int> a;
    list<int> b(10, 1);
    list<int> c(10);
    list<int> d(b);
    list<int> e(l, l + sizeof(l) / sizeof(int));
    for(int i = 0; i < 10; ++i)
    {
        a.push_back(i);
        TEST_ASSERT(a.size()  != i + 1, "invalid list size: %d!", a.size());
        TEST_ASSERT(a.front() != 0, "invalid list front value: %d!", a.front());
        TEST_ASSERT(a.back()  != i, "invalid list back value: %d!", a.back());
        TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
    }
    TEST_ASSERT(a.empty(), "list is not empty!");

    a.insert(++++a.begin(), 0);
    TEST_ASSERT(a[2] != 0, "invalid list value of index 2!");
    TEST_ASSERT(a.size() != 11, "invalid list size: %d!", a.size());

    for(int i = 0; i < 3; ++i)
    {
        a.pop_back();
        TEST_ASSERT(a.size()  != 11 - i - 1, "invalid list size: %d!", a.size());
        TEST_ASSERT(a.front() != 0, "invalid list front value: %d!", a.front());
        TEST_ASSERT(a.back()  != 10 - i - 2, "invalid list back value: %d!", a.back());
        TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
    }
    TEST_ASSERT(a.empty(), "list is not empty!");

    a.erase(++++a.begin());
    TEST_ASSERT(a[2] == 0, "invalid list value of index 2!");
    TEST_ASSERT(a.size() != 7, "invalid list size: %d!", a.size());

    for(int i = 0; i < 3; ++i)
    {
        a.pop_front();
        TEST_ASSERT(a.size()  != 7 - i - 1, "invalid list size: %d!", a.size());
        TEST_ASSERT(a.front() != i + 1, "invalid list front value: %d!", a.front());
        TEST_ASSERT(a.back()  != 6, "invalid list back value: %d!", a.back());
        TEST_ASSERT(a.begin() == a.end(), "invalid iterator!");
    }
}

TEST_CASE(TestList_Speed)
{
    list<int> a;

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