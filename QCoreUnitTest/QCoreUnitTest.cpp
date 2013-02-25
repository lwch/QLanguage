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

using namespace QLanguage;
using namespace QLanguage::Library;
using namespace QLanguage::UnitTest;

#ifdef __MINGW32__
namespace QLanguage
{
    namespace UnitTest
    {
        CaseMap gCaseMap;
    }
}
#endif

size_t iSpeedCaseCount = 0;

inline bool nameFilter(const string& name)
{
    if (name.find("Speed") != -1U)
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
        Console::SetColor(false, true, false, true);
        Console::WriteLine("Run %d cases with time %.03f seconds successed!", iResult, (double)t / 1000);
        Console::SetColor(true, true, true, false);
    }
    catch (const error<string>& e)
    {
        Console::SetColor(true, false, false, true);
        Console::WriteLine(e.description);
    }
    return 0;
}

