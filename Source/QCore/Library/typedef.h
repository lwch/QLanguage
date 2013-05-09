/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:36
	filename: 	\QCore\Library\typedef.h
	file path:	\QCore\Library
	file base:	typedef
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_TYPEDEF_H_
#define _QLANGUAGE_LIBRARY_TYPEDEF_H_

namespace QLanguage
{
    namespace Library
    {
        typedef unsigned short     ushort;
        typedef unsigned int       uint;
        typedef unsigned char      uchar;
        typedef unsigned long      ulong;
        typedef long long          llong;
        typedef unsigned long long ullong;

        template <typename T>
        struct remove_const
        {
            typedef T no_const;
        };

        template <typename T>
        struct remove_const<const T>
        {
            typedef T no_const;
        };

        template <typename T>
        struct remove_reference
        {
            typedef T no_reference;
        };

        template <typename T>
        struct remove_reference<T&>
        {
            typedef T no_reference;
        };

        template <typename T>
        struct remove_unsigned
        {
            typedef T no_unsigned;
            enum { has_unsigned = false };
        };

        template <>
        struct remove_unsigned<uchar>
        {
            typedef char no_unsigned;
            enum { has_unsigned = true };
        };

        template <>
        struct remove_unsigned<ushort>
        {
            typedef short no_unsigned;
            enum { has_unsigned = true };
        };

        template <>
        struct remove_unsigned<uint>
        {
            typedef int no_unsigned;
            enum { has_unsigned = true };
        };

        template <>
        struct remove_unsigned<ulong>
        {
            typedef long no_unsigned;
            enum { has_unsigned = true };
        };

        template <>
        struct remove_unsigned<ullong>
        {
            typedef llong no_unsigned;
            enum { has_unsigned = true };
        };

        template <typename T>
        struct type_pointer
        {
            typedef T* pointer;
        };

        template <typename T>
        struct signed_type
        {
            typedef T type;
        };

        template <>
        struct signed_type<uchar>
        {
            typedef char type;
        };

        template <>
        struct signed_type<ushort>
        {
            typedef short type;
        };

        template <>
        struct signed_type<uint>
        {
            typedef int type;
        };

        template <>
        struct signed_type<ulong>
        {
            typedef long type;
        };

        template <>
        struct signed_type<ullong>
        {
            typedef llong type;
        };

        template <typename T>
        struct unsigned_type
        {
            typedef T type;
        };

        template <>
        struct unsigned_type<char>
        {
            typedef uchar type;
        };

        template <>
        struct unsigned_type<short>
        {
            typedef ushort type;
        };

        template <>
        struct unsigned_type<int>
        {
            typedef uint type;
        };

        template <>
        struct unsigned_type<long>
        {
            typedef ulong type;
        };

        template <>
        struct unsigned_type<llong>
        {
            typedef ullong type;
        };
    }
}

#endif