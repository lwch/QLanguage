#include "TestString.h"

TEST_CASE(TestString)
{
    const char* str = "fortest";
    string a, b(10), c(str), d(str, 3), e(10, 'a'), f(str, str + strlen(str));
    TEST_ASSERT(a.begin() != a.end(), "a's begin iterator is same with end iterator!");
    TEST_ASSERT(a.size() != 0, "a size is 0!");
    TEST_ASSERT(!a.empty(), "a is empty!");

    TEST_ASSERT(b.begin() != b.end(), "b's begin iterator is same with end iterator!");
    TEST_ASSERT(b.size() != 0, "b size is 0!");
    TEST_ASSERT(!b.empty(), "b is empty!");

    TEST_ASSERT(c.end() != c.begin() + strlen(str), "c's end iterator is same with begin iterator + strlen(str)!");
    TEST_ASSERT(c.size() != strlen(str), "c size is strlen(str)!");
    TEST_ASSERT(c.empty(), "c is not empty!");

    TEST_ASSERT(d.end() != d.begin() + 3, "d's end iterator is same with begin iterator + 3!");
    TEST_ASSERT(d.size() != 3, "d size is 3!");
    TEST_ASSERT(d.empty(), "d is not empty!");

    TEST_ASSERT(e.end() != e.begin() + 10, "e's end iterator is same with begin iterator + 10!");
    TEST_ASSERT(e.size() != 10, "e size is 10!");
    TEST_ASSERT(e.empty(), "e is not empty!");

    TEST_ASSERT(f.end() != f.begin() + strlen(str), "f's end iterator is same with begin iterator + strlen(str)!");
    TEST_ASSERT(f.size() != strlen(str), "f size is strlen(str)!");
    TEST_ASSERT(f.empty(), "f is not empty!");

    a.append(b);
    TEST_ASSERT(a != string() + b, "a is string() + b!");

    b.append(c, 0, c.size());
    TEST_ASSERT(b != string(10) + c, "b is string(10) + c!");

    c.append(str);
    TEST_ASSERT(c != string(str) + str, "c is string(str) + str!");

    d.append(str, 0, 3);
    TEST_ASSERT(d != string(str, 3) + "for", "d is string(str, 3) + \"for\"!");

    e.append(f.begin(), f.end());
    TEST_ASSERT(e != string(10, 'a') + f, "e is string(10, 'a') + f!");

    f.append(str, str + strlen(str));
    TEST_ASSERT(f != string(str, str + strlen(str)) + str, "f is string(str, str + strlen(str)) + str!");
}