/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:33
	filename: 	\QCore\Library\hash.h
	file path:	\QCore\Library
	file base:	hash
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_HASH_H_
#define _QLANGUAGE_LIBRARY_HASH_H_

#include "typedef.h"
#include "string.h"

#define HASH_KEY_TYPE ulong
template <typename T>
struct hash
{
    typedef T value_type;

    inline const HASH_KEY_TYPE operator()(const value_type&)const
    {
        return 0;
    }
};

template <typename T>
struct hash<T*>
{
    inline const HASH_KEY_TYPE operator()(T* x)const
    {
        return reinterpret_cast<HASH_KEY_TYPE>(x);
    }
};

template <typename T>
struct hash<const T*>
{
    inline const HASH_KEY_TYPE operator()(const T* x)const
    {
        return reinterpret_cast<HASH_KEY_TYPE>(x);
    }
};

template <>
struct hash<int>
{
    inline const HASH_KEY_TYPE operator()(int x)const
    {
        return x;
    }
};

template <>
struct hash<QLanguage::Library::string>
{
    inline const HASH_KEY_TYPE operator()(const QLanguage::Library::string& x)const
    {
        return reinterpret_cast<HASH_KEY_TYPE>(x.c_str());
    }
};

#endif
