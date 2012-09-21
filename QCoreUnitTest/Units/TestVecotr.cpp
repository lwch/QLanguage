#include "TestVector.h"
#include "../../QCore/Library/string.h"

TEST_CASE(TestVector_Construct)
{
    int l[5] = {1, 2, 3, 4, 5};
    vector<int> a;
    vector<int> b(10, 1);
    vector<int> c(10);
    vector<int> d(b);
    vector<int> e(l, l + sizeof(l) / sizeof(int));
}

TEST_CASE(TestVector_Reserve)
{
    vector<int> a(10, 1);
    a.reserve(20);
    TEST_ASSERT(a.capacity() != 20, "invalid capacity!");
    a.reserve(1);
    TEST_ASSERT(a.capacity() != 20, "invalid capacity!");
}

TEST_CASE(TestVector_Assignment)
{
    vector<int> a;
    vector<int>::iterator b = a.begin();
    TEST_ASSERT(b != NULL, "invalid pointer!");
    vector<int>::const_iterator const c = a.begin();
    TEST_ASSERT(c != NULL, "invalid pointer!");
    vector<int>::reverse_iterator d = a.rbegin();
    TEST_ASSERT(d != NULL, "invalid pointer!");
    vector<int>::const_reverse_iterator const e = a.rbegin();
    TEST_ASSERT(e != NULL, "invalid pointer!");
    vector<int>::iterator f = a.end();
    TEST_ASSERT(f != NULL, "invalid pointer!");
    vector<int>::const_iterator const g = a.end();
    TEST_ASSERT(g != NULL, "invalid pointer!");
    vector<int>::reverse_iterator h = a.rend();
    TEST_ASSERT(h != NULL, "invalid pointer!");
    vector<int>::const_reverse_iterator const i = a.rend();
    TEST_ASSERT(i != NULL, "invalid pointer!");
    vector<int>::reference j = a.front();
    vector<int>::const_reference const k = a.front();
    vector<int>::reference l = a.back();
    vector<int>::const_reference const m = a.back();
    bool const n = a.empty();
    TEST_ASSERT(!n, "a is empty!");
    vector<int>::size_type const o = a.size();
    TEST_ASSERT(o != 0, "invalid size!");
    vector<int>::size_type const p = a.capacity();
    TEST_ASSERT(b != 0, "invalid capacity!");
}

TEST_CASE(TestVector_Operation)
{
    vector<int> a;
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
    vector<int> b(10, 1);
    a = b;
    TEST_ASSERT(a.size() != 10, "invalid size!");
    for(vector<int>::iterator i = a.begin(); i != a.end(); ++i)
    {
        TEST_ASSERT(*i != 1, "invalid value!");
    }
    a[0] = 100;
    TEST_ASSERT(a[0] != 100, "invalid value!");
    const vector<int>::value_type v = a.at(0);
    TEST_ASSERT(v != 100, "invalid value!");
}
