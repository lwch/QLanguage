#ifndef _QLANGUAGE_LIBRARY_MEMORY_H_
#define _QLANGUAGE_LIBRARY_MEMORY_H_

#include <stdlib.h>
#include <stdio.h>

namespace QLanguage
{
    namespace Library
    {
        class MemoryPool
        {
        protected:
            #define ALIGN     sizeof(void*)
            #define MAX_BYTES 128
            #define MAX_COUNT (MAX_BYTES / ALIGN)
        public:
            MemoryPool() : chunk_count(0), free_list(0)
            {
                for(int i = 0; i < MAX_COUNT; ++i) chunk_list[i] = 0;
            }

            ~MemoryPool()
            {
                clear();
            }

            void clear()
            {
                block* current = free_list;
                while(current)
                {
                    block* next = current->next;
                    free(current->data);
                    free(current);
                    current = next;
                }
                free_list = NULL;
            }

            template <typename T>
            T* allocate(size_t n, void(*h)(size_t))
            {
                if(n == 0) return 0;
                if(n > MAX_BYTES)
                {
                    T* p = (T*)malloc(n);
                    while(p == 0)
                    {
                        h(n);
                        p = (T*)malloc(n);
                    }
                    return p;
                }
                const int i = INDEX(ROUND_UP(n));
                obj* p = chunk_list[i];
                if(p == 0)
                {
                    return refill<T>(i, h);
                }
                chunk_list[i] = p->next;
                return reinterpret_cast<T*>(p);
            }

            template <typename T>
            void deallocate(T* p, size_t n)
            {
                if(p == 0) return;
                if(n > MAX_BYTES)
                {
                    free(p);
                    return;
                }
                const int i = INDEX(ROUND_UP(n));
                reinterpret_cast<obj*>(p)->next = chunk_list[i];
                chunk_list[i] = reinterpret_cast<obj*>(p);
            }

            template <typename T>
            T* reallocate(T* p, size_t old_size, size_t new_size, void(*h)(size_t))
            {
                if(old_size > MAX_BYTES && new_size > MAX_BYTES)
                {
                    return realloc(p, new_size);
                }
                if(ROUND_UP(old_size) == ROUND_UP(new_size)) return p;
                T* result = allocate<T>(new_size, h);
                const size_t copy_size = new_size > old_size ? old_size : new_size;
                memcpy(result, p, copy_size);
                deallocate<T>(p, old_size);
                return result;
            }

            void dump()
            {
                FILE* fp = fopen("MemoryPool", "w+");
                for(int i = 0; i < MAX_COUNT; ++i)
                {
                    fprintf(fp, "size: %d count: %d\n", (i + 1) * ALIGN, obj_count(i));
                    obj* current = chunk_list[i];
                    while(current)
                    {
                        fprintf(fp, "0x%08X\n", current);
                        current = current->next;
                    }
                    fprintf(fp, "\n");
                }
                fclose(fp);
            }
        protected:
            struct obj
            {
                obj* next;
            };

            struct block
            {
                block* next;
                void*  data;
            };

            obj*   chunk_list[MAX_COUNT];
            size_t chunk_count;
            block* free_list;

            inline size_t ROUND_UP(size_t bytes)const
            {
                return (bytes + ALIGN - 1) & ~(ALIGN - 1);
            }

            inline size_t ROUND_DOWN(size_t bytes)const
            {
                return bytes & ~(ALIGN - 1);
            }

            inline int INDEX(size_t bytes)const
            {
                return (int)((bytes + ALIGN - 1) / ALIGN - 1);
            }

            inline size_t obj_count(int i)const
            {
                size_t result = 0;
                obj* current = chunk_list[i];
                while(current)
                {
                    ++result;
                    current = current->next;
                }
                return result;
            }

            template <typename T>
            T* refill(int i, void(*h)(size_t))
            {
                const int count = 20;
                const int preSize = (i + 1) * ALIGN;
                char* p = (char*)malloc(preSize * count);
                while(p == 0)
                {
                    h(preSize * count);
                    p = (char*)malloc(preSize * count);
                }
                block* pBlock = (block*)malloc(sizeof(block));
                while(pBlock == 0)
                {
                    h(sizeof(block));
                    pBlock = (block*)malloc(sizeof(block));
                }
                pBlock->data = p;
                pBlock->next = free_list;
                free_list = pBlock;
                obj* current = (obj*)(p + preSize);
                for(int j = 0; j < count - 1; ++j)
                {
                    current->next = chunk_list[i];
                    chunk_list[i] = current;
                    current = (obj*)((char*)current + preSize);
                }
                chunk_count += count - 1;
                //rebalance();
                return reinterpret_cast<T*>(p);
            }

            void rebalance()
            {
                for(int i = MAX_COUNT - 1; i > 0; --i)
                {
                    const size_t avge = chunk_count / MAX_COUNT;
                    size_t count = obj_count(i);
                    if(count > avge)
                    {
                        const size_t preSize = ROUND_DOWN((i + 1) * ALIGN / 2);
                        const int j = INDEX(preSize);
                        for(size_t k = count; k > avge; --k)
                        {
                            obj* chunk = chunk_list[i];
                            chunk_list[i] = chunk_list[i]->next;
                            if(i % 2 == 1)
                            {
                                chunk->next = (obj*)((char*)chunk + preSize);
                                chunk->next->next = chunk_list[j];
                                chunk_list[j] = chunk;
                            }
                            else
                            {
                                chunk->next = chunk_list[j];
                                chunk_list[j] = chunk;
                                obj* next = (obj*)((char*)chunk + preSize);
                                next->next = chunk_list[j + 1];
                                chunk_list[j + 1] = next;
                            }
                            ++chunk_count;
                        }
                    }
                }
            }
        };
    }
}

#endif
