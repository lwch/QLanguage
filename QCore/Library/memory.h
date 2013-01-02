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
protected:
#define ALIGN     sizeof(void*)
#define MAX_BYTES 128
#define MAX_COUNT (MAX_BYTES / ALIGN)

    typedef size_t size_type;
public:
    MemoryPool();
    ~MemoryPool();

    void clear();

    void* allocate(size_type n, void(*h)(size_type));
    void deallocate(void* p, size_type n);
    void* reallocate(void* p, size_t old_size, size_t new_size, void(*h)(size_type));

#ifdef _DEBUG
    void dump();
#endif
protected:
    struct obj
    {
#ifdef _DEBUG
        bool      released;
#endif
        obj*      next;
    };

    struct block
    {
        block* next;
        void*  data;
#ifdef _DEBUG
        size_type size;
#endif
    };

#ifdef _DEBUG
    struct use
    {
        obj* data;
        use* next;
    };
#endif

    obj*      chunk_list[MAX_COUNT];
    block*    free_list;
#ifdef _DEBUG
    use*      use_list;
#endif

    const size_type ROUND_UP(size_type bytes)const;
    const size_type ROUND_DOWN(size_type bytes)const;
    const int INDEX(size_type bytes)const;
    const size_type obj_count(int i)const;

    void* refill(int i, void(*h)(size_type));

#ifdef _DEBUG
    void addUseInfo(obj* ptr);

    enum {headerSize = sizeof(obj) - sizeof(obj*)};
    CallStack callStack;
#else
    enum {headerSize = 0};
#endif
};
#pragma pack()

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
