#include <stdio.h>

#ifdef _WINDOWS
#include <windows.h>
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#include "../QCore/vld/vld.h"
#endif

#include "Units/TestBase.h"

#include "Units/TestList.h"
#include "Units/TestMap.h"
#include "Units/TestMemoryPool.h"
#include "Units/TestRBTree.h"
#include "Units/TestSet.h"
#include "Units/TestString.h"
#include "Units/TestTraits.h"
#include "Units/TestVector.h"

using namespace QLanguage;
using namespace QLanguage::Library;

int retCode = 0;

int main()
{
    return retCode;
}
