/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:35
	filename: 	\QCore\Library\memory.h
	file path:	\QCore\Library
	file base:	memory
	file ext:	h
	author:		lwch

	purpose:
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_MEMORY_H_
#define _QLANGUAGE_LIBRARY_MEMORY_H_

#include "callstack.h"
#include "definition.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
#pragma pack(1)
class MemoryPool
{
    enum { ALIGN = sizeof(void*), MAX_BYTES = 512, MAX_COUNT = MAX_BYTES / ALIGN};
    enum { CALLSTACK_MAX_DEPTH = 30 };

    typedef size_t size_type;

    class Map
    {
        enum { maxBucketLength = 191 };
    public:
        typedef void* key_type;

        struct value_type
        {
            bool        released;
#if DEBUG_LEVEL == 3 // Only windows can get callstack
            UINT_PTR    callStack[CALLSTACK_MAX_DEPTH];
            DWORD       dwCallStackDepth; // Real depth
#endif
        };

        struct bucket_node_type
        {
            key_type          key;
            value_type        value;
            bucket_node_type* next;
        };

        value_type& operator[](const key_type& key);

        template <typename Func>
        const bool for_each(Func func)const
        {
            for (size_type i = 0; i < buckets_count; ++i)
            {
                bucket_node_type* current = buckets[i];
                while (current)
                {
                    bucket_node_type* next = current->next;
                    if (!func(current->key, current->value)) return false;
                    current = next;
                }
            }
            return true;
        }
    protected:
        inline const size_type hash(const key_type& key, const size_type& size)const;

        inline const bool willRehash()const
        {
            for (size_type i = 0; i < buckets_count; ++i)
            {
                if (buckets_length[i] >= maxBucketLength) return true;
            }
            return false;
        }

        void rehash();
    public:
        Map();
        ~Map();
    public:
        bucket_node_type** buckets;
        size_type          buckets_count;
        size_type*         buckets_length;
    };
public:
    MemoryPool();
    ~MemoryPool();

    void clear();

    void* allocate(size_type n, void(*h)(size_type));
    void deallocate(void* p, size_type n);
    void* reallocate(void* p, size_t old_size, size_t new_size, void(*h)(size_type));

    inline const size_type usedMemory()const { return totalSize; }

    inline static MemoryPool* getInstance()
    {
        static MemoryPool pool;
        return &pool;
    }

    const bool hashLeaked()const;
protected:
    struct obj
    {
        obj*      next;
    };

    struct block
    {
        block*    next;
        void*     data;
#ifdef _DEBUG
        size_type size;
#if DEBUG_LEVEL == 3 && defined(WIN32)
        UINT_PTR  callStack[CALLSTACK_MAX_DEPTH];
        DWORD     dwCallStackDepth;
#endif

#endif
    };

    obj*      chunk_list[MAX_COUNT];
    block*    free_list;
    size_type totalSize;
#ifdef _DEBUG
    Map       use_map;
#endif

    const size_type ROUND_UP(size_type bytes)const;
    const size_type ROUND_DOWN(size_type bytes)const;
    const int INDEX(size_type bytes)const;
    const size_type obj_count(int i)const;

    void* refill(int i, void(*h)(size_type));
};
#pragma pack()

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
