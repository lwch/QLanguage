#ifndef _QLANGUAGE_LIBRARY_HASH_H_
#define _QLANGUAGE_LIBRARY_HASH_H_

#include "typedef.h"

namespace QLanguage
{
    namespace Library
    {
#define HASH_KEY_TYPE UINT
        template <typename T>
        class hash
        {
        public:
            typedef T value_type;

            HASH_KEY_TYPE operator()(const value_type&) {}
        };

        template <>
        class hash<int>
        {
        public:
            HASH_KEY_TYPE operator()(int x)
            {
                return x;
            }
        };
    }
}

#endif
