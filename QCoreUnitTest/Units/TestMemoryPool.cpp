#include "TestMemoryPool.h"

TEST_CASE(TestMemoryPool)
{
    MemoryPool pool;
    TIME_START;
    for(int i = 0; i < 128; ++i)
    {
        for(int j = 0; j < 100; ++j)
        {
            void* p = pool.allocate<void>(i, NULL);
            pool.deallocate<void>(p, i);
        }
    }
    SHOW_TIME_COST_SECONDS;
}