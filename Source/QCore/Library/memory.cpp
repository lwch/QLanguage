/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:35
	filename: 	\QCore\Library\memory.cpp
	file path:	\QCore\Library
	file base:	memory
	file ext:	cpp
	author:		lwch

	purpose:
*********************************************************************/

#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "iostream.h"
#include "typedef.h"
#include "memory.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
MemoryPool::Map::Map()
    : buckets_count(11) // 默认吊桶长度为11
{
    size_type count = sizeof(bucket_node_type*) * buckets_count;

    buckets = (bucket_node_type**)malloc(count);
    memset(buckets, 0, count);

    count = sizeof(size_type) * buckets_count;
    buckets_length = (size_type*)malloc(count);
    memset(buckets_length, 0, count);
}

MemoryPool::Map::~Map()
{
    for (size_type i = 0; i < buckets_count; ++i)
    {
        bucket_node_type *current = buckets[i];
        while (current)
        {
            bucket_node_type* next = current->next;
            destruct(current, has_destruct(*current));
            free(current);
            current = next;
        }
    }
    destruct_range(buckets_length, buckets_length + buckets_count);
    free(buckets_length);
}

MemoryPool::Map::value_type& MemoryPool::Map::operator[](const key_type& key)
{
    if (willRehash()) rehash();

    const size_type idx = hash(key, buckets_count);

    bucket_node_type* ptr = NULL;
    bucket_node_type* current = buckets[idx];

    while (current)
    {
        if (current->key == key)
        {
            ptr = current;
            break;
        }
        current = current->next;
    }

    if (ptr == NULL)
    {
        ptr = (bucket_node_type*)malloc(sizeof(bucket_node_type));

        ptr->key = key;
        ptr->value.released = true;
#if DEBUG_LEVEL == 3 && defined(_DEBUG)
        memset(ptr->value.callStack, 0, CALLSTACK_MAX_DEPTH * sizeof(UINT_PTR));
        ptr->value.dwCallStackDepth = CallStack::getInstance().stackTrace(ptr->value.callStack, CALLSTACK_MAX_DEPTH);
#endif
        ptr->next = buckets[idx];
        buckets[idx] = ptr;
        ++buckets_length[idx];
    }
    return ptr->value;
}

void MemoryPool::Map::rehash()
{
    size_type newCount = buckets_count << 1;

    size_type count = sizeof(bucket_node_type*) * newCount;
    bucket_node_type** newBuckets = (bucket_node_type**)malloc(count);
    memset(newBuckets, 0, count);

    count = sizeof(size_type) * newCount;
    buckets_length = (size_type*)realloc(buckets_length, count);
    memset(buckets_length, 0, count);

    for (size_type i = 0; i < buckets_count; ++i)
    {
        bucket_node_type* current = buckets[i];
        while (current)
        {
            bucket_node_type* next = current->next;

            const size_type idx = hash(current->key, newCount);
            current->next = newBuckets[idx];
            newBuckets[idx] = current;

            ++buckets_length[idx];

            current = next;
        }
    }

    destruct_range(buckets, buckets + buckets_count);
    free(buckets);

    buckets = newBuckets;
    buckets_count = newCount;
}

inline const MemoryPool::size_type MemoryPool::Map::hash(const key_type& key, const size_type& size)const
{
    return hash_seq((const uchar*)&key, sizeof(key_type)) % size;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
MemoryPool::MemoryPool()
    : free_list(NULL)
    , totalSize(0)
{
    for (uint i = 0; i < MAX_COUNT; ++i) chunk_list[i] = 0;
}

MemoryPool::~MemoryPool()
{
#ifdef _DEBUG
    use_map.for_each(
    [](void* k, const Map::value_type& v) -> bool
    {
        if (!v.released)
        {
#if DEBUG_LEVEL == 3
            for (DWORD i = 0; i < v.dwCallStackDepth; ++i)
            {
                CallStack::FuncInfo funcInfo;
                CallStack::getInstance().getFuncInfo(v.callStack[i], funcInfo);
                cerr << string::format("MemoryLeaked: %s", funcInfo.szFuncName) << endl;
                cerr << string::format("File: %s on line %d", funcInfo.szFilePath, funcInfo.dwLineNumber) << endl;
            }
#endif
            throw error<const char*>("chunk leaked", __FILE__, __LINE__);
        }
        return v.released;
    });
#endif
    clear();
}

void MemoryPool::clear()
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

void* MemoryPool::allocate(size_type n, void(*h)(size_type))
{
    if (n == 0) return 0;
    if (n > MAX_BYTES)
    {
        void* p = malloc(n);
        while (p == 0)
        {
            h(n);
            p = malloc(n);
        }
#ifdef _DEBUG
        use_map[p].released = false;
#endif
        totalSize += n;
        return p;
    }
    const size_t size = ROUND_UP(n);
    const int i = INDEX(size);
    obj* p = chunk_list[i];
    if (p == 0)
    {
        for (int j = i + 1; j < MAX_COUNT; ++j)
        {
            p = chunk_list[j];
            if (p != NULL)
            {
                chunk_list[j] = chunk_list[j]->next;
                const int l = INDEX((j + 1) * ALIGN - size);
                obj* q = (obj*)((char*)p + size);
                q->next = chunk_list[l];
                chunk_list[l] = q;
#ifdef _DEBUG
                use_map[q].released = true;
                use_map[p].released = false;
#endif
                totalSize += n;
                return (char*)p;
            }
        }
        return refill(i, h);
    }
    chunk_list[i] = p->next;
#ifdef _DEBUG
    use_map[p].released = false;
#endif
    totalSize += n;
    return (char*)p;
}

void MemoryPool::deallocate(void* p, size_type n)
{
    if (p == 0) return;
    if (n > MAX_BYTES)
    {
        free(p);
#ifdef _DEBUG
        use_map[p].released = true;
#endif
        totalSize -= n;
        return;
    }
    const int i = INDEX(ROUND_UP(n));
    reinterpret_cast<obj*>(p)->next = chunk_list[i];
    chunk_list[i] = reinterpret_cast<obj*>(p);
#ifdef _DEBUG
    use_map[p].released = true;
#endif
    totalSize -= n;
}

void* MemoryPool::reallocate(void* p, size_t old_size, size_t new_size, void(*h)(size_type))
{
    if (old_size > MAX_BYTES && new_size > MAX_BYTES)
    {
        return realloc(p, new_size);
    }
    if (ROUND_UP(old_size) == ROUND_UP(new_size)) return p;
    void* result = allocate(new_size, h);
    const size_t copy_size = new_size > old_size ? old_size : new_size;
    memcpy(result, p, copy_size);
    deallocate(p, old_size);
    return result;
}

const bool MemoryPool::hashLeaked()const
{
#ifdef _DEBUG
    return use_map.for_each(
           [](void* k, const Map::value_type& v) -> bool
           {
               return v.released;
           });
#else
    return false;
#endif
}

inline const size_t MemoryPool::ROUND_UP(size_type bytes)const
{
    return (bytes + ALIGN - 1) & ~(ALIGN - 1);
}

inline const size_t MemoryPool::ROUND_DOWN(size_type bytes)const
{
    return bytes & ~(ALIGN - 1);
}

inline const int MemoryPool::INDEX(size_type bytes)const
{
    return (int)((bytes + ALIGN - 1) / ALIGN - 1);
}

const size_t MemoryPool::obj_count(int i)const
{
    size_t result = 0;
    obj* current = chunk_list[i];
    while (current)
    {
        ++result;
        current = current->next;
    }
    return result;
}

void* MemoryPool::refill(int i, void(*h)(size_type))
{
    const int count = 20;
    const int preSize = (i + 1) * ALIGN;
    char* p = (char*)malloc(preSize * count);
    while (p == 0)
    {
        h(preSize * count);
        p = (char*)malloc(preSize * count);
    }
    block* pBlock = (block*)malloc(sizeof(block));
    while (pBlock == 0)
    {
        h(sizeof(block));
        pBlock = (block*)malloc(sizeof(block));
    }
#if DEBUG_LEVEL == 3 && defined(_DEBUG) && defined(WIN32)
    memset(pBlock->callStack, 0, CALLSTACK_MAX_DEPTH * sizeof(UINT_PTR));
    pBlock->dwCallStackDepth = CallStack::getInstance().stackTrace(pBlock->callStack, CALLSTACK_MAX_DEPTH);
#endif
    pBlock->data = p;
    pBlock->next = free_list;
    free_list = pBlock;

    obj* current = (obj*)p;
#ifdef _DEBUG
    use_map[p].released = false;
#endif
    current = (obj*)((char*)current + preSize);
    for (int j = 0; j < count - 1; ++j)
    {
#ifdef _DEBUG
        use_map[current].released = true;
#endif
        current->next = chunk_list[i];
        chunk_list[i] = current;
        current = (obj*)((char*)current + preSize);
    }
    return p;
}

NAMESPACE_QLANGUAGE_LIBRARY_END
