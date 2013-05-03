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
        };

        template <>
        struct remove_unsigned<uchar>
        {
            typedef char no_unsigned;
        };

        template <>
        struct remove_unsigned<ushort>
        {
            typedef short no_unsigned;
        };

        template <>
        struct remove_unsigned<uint>
        {
            typedef int no_unsigned;
        };

        template <>
        struct remove_unsigned<ulong>
        {
            typedef long no_unsigned;
        };

        template <>
        struct remove_unsigned<ullong>
        {
            typedef llong no_unsigned;
        };
    }
}

#endif