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

inline HASH_KEY_TYPE hash_seq(const uchar* first, size_t size)
{
#ifdef X86
    ulong offset_basis = 2166136261U;
    ulong prime        = 16777619U;
#else
    ulong offset_basis = 14695981039346656037U;
    ulong prime        = 1099511628211U;
#endif

    size_t value = offset_basis;
    for (size_t i = 0; i < size; ++i)
    {
        value ^= (size_t)first[i];
        value *= prime;
    }

#ifdef X64
    value ^= value >> 32;
#endif

    return value;
}

template <typename T>
struct hash
{
    typedef T value_type;
};

template <typename T>
struct hash_base
{
    inline const HASH_KEY_TYPE operator()(const T& x)const
    {
        return hash_seq((const uchar*)&x, sizeof(T));
    }
};

template <typename T>
struct hash<T*> : hash_base<T*>
{
};

template <typename T>
struct hash<const T*> : hash_base<const T*>
{
};

template <>
struct hash<char> : hash_base<char>
{
};

template <>
struct hash<short> : hash_base<short>
{
};

template <>
struct hash<int> : hash_base<short>
{
};

template <>
struct hash<long> : hash_base<long>
{
};

template <>
struct hash<llong> : hash_base<llong>
{
};

template <>
struct hash<uchar> : hash_base<uchar>
{
};

template <>
struct hash<ushort> : hash_base<ushort>
{
};

template <>
struct hash<uint> : hash_base<uint>
{
};

template <>
struct hash<ulong> : hash_base<ulong>
{
};

template <>
struct hash<ullong> : hash_base<ullong>
{
};

template <>
struct hash<QLanguage::Library::string>
{
    inline const HASH_KEY_TYPE operator()(const QLanguage::Library::string& x)const
    {
        return hash_seq(reinterpret_cast<const uchar*>(x.c_str()), x.size());
    }
};

#endif
