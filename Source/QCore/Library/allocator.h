/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:31
	filename: 	\QCore\Library\allocator.h
	file path:	\QCore\Library
	file base:	allocator
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_ALLOC_H_
#define _QLANGUAGE_LIBRARY_ALLOC_H_

#if 0
#include <new>
#define __THROW_BAD_ALLOC throw std::bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include <iostream>
#define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)
#endif

#include "memory.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        class allocator_min
        {
        public:
            allocator_min()
            {
            }

            allocator_min(const allocator_min<T>&)
            {
            }

            static T* allocate()
            {
                const size_t size = sizeof(T);
                T* result = (T*)malloc(size);
                while (result == 0)
                {
                    if(free_handler) free_handler(size);
                    else __THROW_BAD_ALLOC;
                    result = (T*)malloc(size);
                }
                totalSize += size;
                return result;
            }

            static T* allocate(size_t n)
            {
                const size_t size = n * sizeof(T);
                if(size < 0) throw "bad allocate size";
                T* result = (T*)malloc(size);
                while(result == 0)
                {
                    if(free_handler) free_handler(size);
                    else __THROW_BAD_ALLOC;
                    result = (T*)malloc(size);
                }
                totalSize += size;
                return result;
            }

            static void deallocate(T* p)
            {
                free(p);
                totalSize -= sizeof(T);
            }

            static void deallocate(T* p, size_t n)
            {
                free(p);
                totalSize -= sizeof(T) * n;
            }

            static void deallocateWithSize(T* p, size_t n)
            {
                free(p);
                totalSize -= n;
            }

            static T* reallocate(T* p, size_t old_size, size_t n)
            {
                const size_t size = n * sizeof(T);
                if(size <= 0) throw "bad reallocate size";
                T* result = (T*)realloc(p, size);
                while(result == 0)
                {
                    if(free_handler) free_handler(size);
                    else __THROW_BAD_ALLOC;
                    result = (T*)realloc(p, size);
                }
                totalSize -= old_size;
                totalSize += size;
                return result;
            }

            inline static const size_t usedMemory()
            {
                return totalSize;
            }
        public:
            static void(*free_handler)(size_t);
            static size_t totalSize;

            static void set_handler(void(*h)(size_t))
            {
                free_handler = h;
            }
        };

        template <typename T>
        void (*allocator_min<T>::free_handler)(size_t) = 0;

        template <typename T>
        size_t allocator_min<T>::totalSize = 0;

        template <typename T>
        class allocator
        {
        public:
            allocator()
            {
            }

            allocator(const allocator<T>&)
            {
            }

            inline static T* allocate()
            {
                MemoryPool* pool = MemoryPool::getInstance();
                T* p = reinterpret_cast<T*>(pool->allocate(sizeof(T), free_handler));
                return p;
            }

            static T* allocate(size_t n)
            {
                MemoryPool* pool = MemoryPool::getInstance();
                return reinterpret_cast<T*>(pool->allocate(n * sizeof(T), free_handler));
            }

            inline static void deallocate(T* p)
            {
                MemoryPool* pool = MemoryPool::getInstance();
                pool->deallocate(p, sizeof(T));
            }

            inline static void deallocate(T* p, size_t n)
            {
                MemoryPool* pool = MemoryPool::getInstance();
                pool->deallocate(p, n * sizeof(T));
            }

            inline static void deallocateWithSize(T* p, size_t n)
            {
                MemoryPool* pool = MemoryPool::getInstance();
                pool->deallocate(p, n);
            }

            static T* reallocate(T* p, size_t old_size, size_t n)
            {
                MemoryPool* pool = MemoryPool::getInstance();
                p = pool->reallocate(p, old_size, n * sizeof(T), free_handler);
                return p;
            }

            inline static const size_t usedMemory()
            {
                return MemoryPool::getInstance()->usedMemory();
            }
        public:
            static void(*free_handler)(size_t);

            static void set_handler(void(*h)(size_t))
            {
                free_handler = h;
            }
        };

        template <typename T>
        void (*allocator<T>::free_handler)(size_t) = 0;
    }
}

#endif
