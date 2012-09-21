#include "TestList.h"

class A
{
};

class B
{
};

TEST_CASE(TestList_Construct)
{
    int l[5] = {1, 2, 3, 4, 5};
    list<int> a;
    list<int> b(10, 1);
    list<int> c(10);
    list<int> d(b);
    list<int> e(l, l + sizeof(l) / sizeof(int));
}

TEST_CASE(TestList_Assignment)
{
    list<int> a;
    list<int>::iterator b = a.begin();
    TEST_ASSERT(b == NULL, "invalid pointer!");
    list<int>::const_iterator const c = a.begin();
    TEST_ASSERT(c == NULL, "invalid pointer!");
    list<int>::reverse_iterator d = a.rbegin();
    TEST_ASSERT(d != a.rend(), "invalid pointer!");
    list<int>::const_reverse_iterator const e = a.rbegin();
    TEST_ASSERT(e != a.rend(), "invalid pointer!");
    list<int>::iterator f = a.end();
    TEST_ASSERT(f == NULL, "invalid pointer!");
    list<int>::const_iterator const g = a.end();
    TEST_ASSERT(g == NULL, "invalid pointer!");
    list<int>::reverse_iterator h = a.rend();
    TEST_ASSERT(h != a.rbegin(), "invalid pointer!");
    list<int>::const_reverse_iterator const i = a.rend();
    TEST_ASSERT(i != a.rbegin(), "invalid pointer!");
    list<int>::reference j = a.front();
    list<int>::const_reference const k = a.front();
    list<int>::reference l = a.back();
    list<int>::const_reference const m = a.back();
    bool const n = a.empty();
    TEST_ASSERT(!n, "a is empty!");
    list<int>::size_type const o = a.size();
    TEST_ASSERT(o != 0, "invalid size!");
}

TEST_CASE(TestList_Operation)
{
    list<int> a;
    a.push_back(1);
    TEST_ASSERT(a.size() != 1, "invalid size!");
    TEST_ASSERT(a[0] != 1, "invalid value!");
    a.pop_back();
    TEST_ASSERT(a.size() != 0, "invalid size!");
    a.insert(a.begin(), 1);
    TEST_ASSERT(a.size() != 1, "invalid size!");
    TEST_ASSERT(a[0] != 1, "invalid value!");
    a.erase(a.begin());
    TEST_ASSERT(a.size() != 0, "invalid size!");
    list<int> b(10, 1);
    a = b;
    TEST_ASSERT(a.size() != 10, "invalid size!");
    for(list<int>::iterator i = a.begin(); i != a.end(); ++i)
    {
        TEST_ASSERT(*i != 1, "invalid value!");
    }
    a[0] = 100;
    TEST_ASSERT(a[0] != 100, "invalid value!");
    const list<int>::value_type v = a.at(0);
    TEST_ASSERT(v != 100, "invalid value!");
}
