// QLanguage.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <time.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#include "../QCore/vld/vld.h"
#endif

#include "../QCore/Library/vector.h"
#include "../QCore/Library/list.h"
#include "../QCore/Library/rbtree.h"
#include "../QCore/Library/set.h"
#include "../QCore/Library/map.h"
#include "../QCore/Library/string.h"

int main()
{
    QLanguage::Library::rbtree<int, int, QLanguage::Library::identity<int> > t;
    QLanguage::Library::set<int> s;
    QLanguage::Library::map<int, int> m;

    QLanguage::Library::vector<int> v, q;
    QLanguage::Library::list<int> p;
    for (int i = 0; i < 10; i++)
    {
        i >> p << i;
        v << i;
        for (QLanguage::Library::vector<int>::iterator j = v.begin(); j != v.end(); ++j)
        {
            q << *j;
            p << *j;
        }
        p.pop_front();
        p.pop_back();
    }
    v.pop_back();
    v.erase(v.begin());
    p.erase(p.begin());

//     for(int i = 0; i < 10000; ++i)
//     {
//         for(int j = 4; j <= 128; j += 4)
//         {
//             QLanguage::Library::__MemoryPool__1.allocate<int>(j, 0);
//         }
//     }

    t << QLanguage::Library::pair<bool, const int&>(false, 30)
        << QLanguage::Library::pair<bool, const int&>(false, 15)
        << QLanguage::Library::pair<bool, const int&>(false, 70)
        << QLanguage::Library::pair<bool, const int&>(false, 10)
        << QLanguage::Library::pair<bool, const int&>(false, 20)
        << QLanguage::Library::pair<bool, const int&>(false, 60)
        << QLanguage::Library::pair<bool, const int&>(false, 85)
        << QLanguage::Library::pair<bool, const int&>(false, 5)
        << QLanguage::Library::pair<bool, const int&>(false, 25)
        << QLanguage::Library::pair<bool, const int&>(false, 50)
        << QLanguage::Library::pair<bool, const int&>(false, 65)
        << QLanguage::Library::pair<bool, const int&>(false, 80)
        << QLanguage::Library::pair<bool, const int&>(false, 90)
        << QLanguage::Library::pair<bool, const int&>(false, 40)
        << QLanguage::Library::pair<bool, const int&>(false, 55);

    t.erase(15);
    t.erase(70);
    t.erase(10);

    for(int i = 0; i < 100; ++i)
    {
        s.insert(i);
        m[i] = i;
    }

    printf("%d\n", m[55]);

    QLanguage::Library::string str = "bbbbaaaaa";

    size_t ia = str.find("ccc");

    //QLanguage::Library::__MemoryPool__1.dump();

    return 0;
}
