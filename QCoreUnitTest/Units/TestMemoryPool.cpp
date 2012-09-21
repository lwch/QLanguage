#include "TestMemoryPool.h"

TEST_CASE(TestMemoryPool)
{
    MemoryPool pool;
    for(int i = 0; i < 128; ++i)
    {
        for(int j = 0; j < 100; ++j)
        {
            void* p = pool.allocate<void>(i, NULL);
            pool.deallocate<void>(p, i);
        }
    }
}