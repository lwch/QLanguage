/********************************************************************
	created:	2013/04/11
	created:	11:4:2013   10:15
	filename: 	\QCore\Library\almighty_container.h
	file path:	\QCore\Library
	file base:	almighty_container
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_ALMIGHTY_CONTAINER_H_
#define _QLANGUAGE_LIBRARY_ALMIGHTY_CONTAINER_H_

#include "definition.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    class almighty_container
    {
    public:
        typedef T value_type;

        almighty_container(const value_type& x) : pointer(&x) {}
        virtual ~almighty_container() {}
    public:
        const value_type* pointer;
    };

    template <typename T>
    class almighty_container<T&>
    {
    public:
        typedef T value_type;

        almighty_container(value_type& x) : pointer(&x) {}
        virtual ~almighty_container() {}
    public:
        const value_type* pointer;
    };

    template <typename T>
    class almighty_container<const T&>
    {
    public:
        typedef T value_type;

        almighty_container(const value_type& x) : pointer(&x) {}
        virtual ~almighty_container() {}
    public:
        const value_type* pointer;
    };

    template <typename T>
    class almighty_container<T*>
    {
    public:
        typedef T value_type;

        almighty_container(T* x) : pointer(x) {}
        virtual ~almighty_container() {}
    public:
        const value_type* pointer;
    };

    template <typename T>
    class almighty_container<const T*>
    {
    public:
        typedef T value_type;

        almighty_container(const T* x) : pointer(x) {}
        virtual ~almighty_container() {}
    public:
        const value_type* pointer;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
