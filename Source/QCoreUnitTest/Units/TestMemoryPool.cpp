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

#include "../../QCore/Library/fstream.h"
#include "TestMemoryPool.h"

namespace QLanguage
{
    namespace UnitTest
    {
        TEST_CASE(TestMemoryPool_Speed)
        {
            fstream stream("memory_pool_report.txt", fstream::out);
            MemoryPool pool;

            const size_t size = 1000;
            struct type1
            {
                long a; long b; long c; long d;
                long e; long f; long g; long h;
                long i; long j; long k; long l;
                long m; long n; long o; long p;

                type1() {}
            };

            struct type2
            {
                type1 a; type1 b; type1 c; type1 d;
                type1 e; type1 f; type1 g; type1 h;
                type1 i; type1 j; type1 k; type1 l;
                type1 m; type1 n; type1 o; type1 p;

                type2() {}
            };

            struct type
            {
                type2 a;
                type2 b;

                type() {}
            };

            for (size_t j = 0; j < 10; ++j)
            {
                clock_t start = clock();
                for (size_t i = 0; i < size; ++i)
                {
                    type* ptr = static_cast<type*>(pool.allocate(sizeof(type), NULL));
                    construct(ptr);
                    destruct(ptr, has_destruct(*ptr));
                    pool.deallocate(ptr, sizeof(type));
                }
                stream << (clock() - start) * 1000 / CLOCKS_PER_SEC << ',';
                start = clock();
                for (size_t i = 0; i < size; ++i)
                {
                    type* ptr = new type;
                    delete ptr;
                }
                stream << (clock() - start) * 1000 / CLOCKS_PER_SEC << endl;
            }
        }
    }
}
