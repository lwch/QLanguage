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

namespace QLanguage
{
    namespace Library
    {
#define HASH_KEY_TYPE ulong
        template <typename T>
        class hash
        {
        public:
            typedef T value_type;

            inline const HASH_KEY_TYPE operator()(const value_type&)const
            {
                return 0;
            }
        };

        template <typename T>
        class hash<T*>
        {
        public:
            inline const HASH_KEY_TYPE operator()(T* x)const
            {
                return reinterpret_cast<HASH_KEY_TYPE>(x);
            }
        };

        template <typename T>
        class hash<const T*>
        {
        public:
            inline const HASH_KEY_TYPE operator()(const T* x)const
            {
                return reinterpret_cast<HASH_KEY_TYPE>(x);
            }
        };

        template <>
        class hash<int>
        {
        public:
            inline const HASH_KEY_TYPE operator()(int x)const
            {
                return x;
            }
        };

        template <>
        class hash<string>
        {
        public:
            inline const HASH_KEY_TYPE operator()(const string& x)const
            {
                return reinterpret_cast<HASH_KEY_TYPE>(x.c_str());
            }
        };
    }
}

#endif
