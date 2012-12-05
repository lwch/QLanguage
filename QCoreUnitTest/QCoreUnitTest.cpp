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

#if defined(_DEBUG) && defined(WIN32)
#include "../QCore/vld/vld.h"
#endif

#include "Units/TestBase.h"

#include "Units/TestHashTable.h"
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
