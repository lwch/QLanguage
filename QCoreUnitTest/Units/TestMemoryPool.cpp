/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:39
	filename: 	\QCoreUnitTest\Units\TestMemoryPool.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestMemoryPool
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/

#include "TestMemoryPool.h"

TEST_CASE(TestMemoryPool)
{
    MemoryPool pool;
    TIME_START;
    for(int i = 0; i < 128; ++i)
    {
        for(int j = 0; j < 100; ++j)
        {
            void* p = pool.allocate(i, NULL);
            pool.deallocate(p, i);
        }
    }
    SHOW_TIME_COST_SECONDS;
}