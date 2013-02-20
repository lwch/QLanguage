/********************************************************************
	created:	2013/02/17
	created:	17:2:2013   21:35
	filename: 	\QCoreUnitTest\Units\TestBase.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestBase
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/

#include "TestBase.h"

#ifndef __MINGW32__
// Fuck MinGW with error!!!!
namespace QLanguage
{
    namespace UnitTest
    {
        CaseMap gCaseMap;
    }
}
#endif
