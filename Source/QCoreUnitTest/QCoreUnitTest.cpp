 /********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:37
	filename: 	\QCoreUnitTest\QCoreUnitTest.cpp
	file path:	\QCoreUnitTest
	file base:	QCoreUnitTest
	file ext:	cpp
	author:		lwch

	purpose:
*********************************************************************/

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "Units/TestBase.h"

#include "Units/TestCombinator.h"
#include "Units/TestHashTable.h"
#include "Units/TestList.h"
#include "Units/TestMap.h"
#include "Units/TestMemoryPool.h"
#include "Units/TestRBTree.h"
#include "Units/TestRegex.h"
#include "Units/TestSet.h"
#include "Units/TestString.h"
#include "Units/TestTraits.h"
#include "Units/TestVector.h"

#include "../QCore/Library/graph/bitmap.h"

using namespace QLanguage;
using namespace QLanguage::Library;
using namespace QLanguage::UnitTest;

size_t iSpeedCaseCount = 0;

inline bool nameFilter(const string& name)
{
    if (name.find("Speed") != string::npos)
    {
        ++iSpeedCaseCount;
#ifdef TEST_SPEED
        return true;
#else
        return false;
#endif
    }
    return true;
}

int main()
{
    SET_NAME_FILTER(nameFilter);

    clock_t t = clock();
    size_t iResult = RUN_ALL_CASE;
    t = clock() - t;

    size_t iCount  = ALL_CASE_COUNT;
    try
    {
#ifdef TEST_SPEED
        TEST_ASSERT(iResult != iCount, "some error!");
#else
        TEST_ASSERT(iResult != (iCount - iSpeedCaseCount), "some error!");
#endif
        cout.setColor(cout.lightWith(stdstream::green));
        cout << string::format("Run %d cases with time %.03f seconds successed!", iResult, (double)t / 1000) << endl;
        cout.setColor(stdstream::white);
    }
    catch (const error<const char*>& e)
    {
        cerr << e.description;
    }

//     using namespace QLanguage::Library::graph;
//     _RGBQUAD q[256];
//     srand(clock());
//     for (int i = 0; i < 256; ++i)
//     {
//         q[i].blue  = rand() % 256;
//         q[i].green = rand() % 256;
//         q[i].red   = rand() % 256;
//     }
//     Bitmap bitmap(1000, 1000, Bitmap::Index8, q);
//     uchar* p = bitmap.line(0);
//     for (size_t i = 0; i < 1000 * 1000; ++i)
//     {
//         p[i] = rand() % 256;
//     }
//     bitmap.save("test.bmp");
    return 0;
}

