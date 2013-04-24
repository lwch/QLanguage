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

#include "Console.h"
#include "typedef.h"
#include "memory.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
///////////////////////////////////////////////////////////////////////////////////////////////////
MemoryPool::MemoryPool() : free_list(NULL)
{
    for(uint i = 0; i < MAX_COUNT; ++i) chunk_list[i] = 0;

#ifdef _DEBUG
    use_list = NULL;
#endif
}

MemoryPool::~MemoryPool()
{
#ifdef _DEBUG
    while (use_list)
    {
        use *ptr = use_list, *next = use_list->next;
        if (!ptr->data->released)
        {
            Console::SetColor(true, false, false, true);
#if DEBUG_LEVEL == 3 && defined(_DEBUG) && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
            obj* pObj = ptr->data;
            for (DWORD j = 0; j < pObj->dwCallStackDepth; ++j)
            {
                CallStack::FuncInfo funcInfo;
                CallStack::getInstance().getFuncInfo(pObj->callStack[j], funcInfo);
                printf("MemoryLeaked: %s\nFile: %s in line %d\n", funcInfo.szFuncName, funcInfo.szFilePath, funcInfo.dwLineNumber);
            }
#endif
            throw error<const char*>("chunk leaked", __FILE__, __LINE__);
        }
        free(ptr);
        use_list = next;
    }
#endif
    clear();
}

void MemoryPool::clear()
{
    block* current = free_list;
    while(current)
    {
        block* next = current->next;
        // why add try catch is ok?
        try
        {
            free(current->data);
            free(current);
        }
        catch (...)
        {
#if DEBUG_LEVEL == 3 && defined(_DEBUG) && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
            for (DWORD i = 0; i < current->dwCallStackDepth; ++i)
            {
                CallStack::FuncInfo info;
                CallStack::getInstance().getFuncInfo(current->callStack[i], info);
                printf("ERROR: %s\n%s on line %d", info.szFuncName, info.szFilePath, info.dwLineNumber);
            }
#endif
        }
        current = next;
    }
    free_list = NULL;
}

void* MemoryPool::allocate(size_type n, void(*h)(size_type))
{
    if(n == 0) return 0;
    if(n > MAX_BYTES)
    {
        void* p = malloc(n);
        while(p == 0)
        {
            h(n);
            p = malloc(n);
        }
        return p;
    }
    const size_t size = ROUND_UP(n);
    const int i = INDEX(size);
    obj* p = chunk_list[i];
    if(p == 0)
    {
#ifdef NDEBUG
        for(int j = i + 1; j < MAX_COUNT; ++j)
        {
            p = chunk_list[j];
            if(p != 0)
            {
                chunk_list[j] = chunk_list[j]->next;
                const int l = INDEX((j + 1) * ALIGN - size);
                obj* q = (obj*)((char*)p + size + headerSize);
                q->next = chunk_list[l];
                chunk_list[l] = q;
                return (char*)p + headerSize;
            }
        }
#endif
        return refill(i, h);
    }
#ifdef _DEBUG

#if DEBUG_LEVEL == 3 && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
    memset(chunk_list[i]->callStack, 0, CALLSTACK_MAX_DEPTH * sizeof(UINT_PTR));
    chunk_list[i]->dwCallStackDepth = CallStack::getInstance().stackTrace(chunk_list[i]->callStack, CALLSTACK_MAX_DEPTH);
#endif
    chunk_list[i]->released = false;
#endif
    chunk_list[i] = p->next;
    return (char*)p + headerSize;
}

void MemoryPool::deallocate(void* p, size_type n)
{
    if(p == 0) return;
    if(n > MAX_BYTES)
    {
        free(p);
        return;
    }
    const int i = INDEX(ROUND_UP(n));
#ifdef _DEBUG
    p = (char*)p - (int)headerSize;
    obj* ptr = reinterpret_cast<obj*>(p);
#if DEBUG_LEVEL == 3 && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
//     for (DWORD j = 0; j < ptr->dwCallStackDepth; ++j)
//     {
//         CallStack::FuncInfo funcInfo;
//         callStack.getFuncInfo(ptr->callStack[j], funcInfo);
//         printf("MemoryLeaked: %s\nFile: %s in line %d\n", funcInfo.szFuncName, funcInfo.szFilePath, funcInfo.dwLineNumber);
//     }
    memset(ptr->callStack, 0, CALLSTACK_MAX_DEPTH * sizeof(UINT_PTR));
#endif
    if (ptr->released) throw error<const char*>("chunk has already released", __FILE__, __LINE__);
    ptr->released = true;
#endif
    reinterpret_cast<obj*>(p)->next = chunk_list[i];
    chunk_list[i] = reinterpret_cast<obj*>(p);
}

void* MemoryPool::reallocate(void* p, size_t old_size, size_t new_size, void(*h)(size_type))
{
    if(old_size > MAX_BYTES && new_size > MAX_BYTES)
    {
        return realloc(p, new_size);
    }
    if(ROUND_UP(old_size) == ROUND_UP(new_size)) return p;
    void* result = allocate(new_size, h);
    const size_t copy_size = new_size > old_size ? old_size : new_size;
    memcpy(result, p, copy_size);
    deallocate(p, old_size);
    return result;
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
    while(current)
    {
        ++result;
        current = current->next;
    }
    return result;
}

void* MemoryPool::refill(int i, void(*h)(size_type))
{
    const int count = 20;
    const int preSize = (i + 1) * ALIGN + headerSize;
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
#if DEBUG_LEVEL == 3 && defined(_DEBUG) && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
    memset(pBlock->callStack, 0, CALLSTACK_MAX_DEPTH * sizeof(UINT_PTR));
    pBlock->dwCallStackDepth = CallStack::getInstance().stackTrace(pBlock->callStack, CALLSTACK_MAX_DEPTH);
#endif
    pBlock->data = p;
    pBlock->next = free_list;
    free_list = pBlock;

    obj* current = (obj*)p;
#ifdef _DEBUG
    addUseInfo(current);
#if DEBUG_LEVEL == 3 && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
    memset(current->callStack, 0, CALLSTACK_MAX_DEPTH * sizeof(UINT_PTR));
    current->dwCallStackDepth = CallStack::getInstance().stackTrace(current->callStack, CALLSTACK_MAX_DEPTH);
#endif
    current->released = false;
#endif
    current = (obj*)((char*)current + preSize);
    for(int j = 0; j < count - 1; ++j)
    {
#ifdef _DEBUG
        addUseInfo(current);
        current->released = true;
#endif
        current->next = chunk_list[i];
        chunk_list[i] = current;
        current = (obj*)((char*)current + preSize);
    }
    return (char*)p + headerSize;
}

#ifdef _DEBUG
inline void MemoryPool::addUseInfo(obj* ptr)
{
    use* p = (use*)malloc(sizeof(use));
    p->data = ptr;
    p->next = use_list;
    use_list = p;
}
#endif

NAMESPACE_QLANGUAGE_LIBRARY_END
