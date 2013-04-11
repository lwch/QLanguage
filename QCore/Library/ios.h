/********************************************************************
	created:	2013/04/08
	created:	8:4:2013   16:07
	filename: 	\QCore\Library\ios.h
	file path:	\QCore\Library
	file base:	ios
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_IOS_H_
#define _QLANGUAGE_LIBRARY_IOS_H_

#include <limits>

#include "definition.h"
#include "string.h"
#include "almighty_container.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    class basic_ios
    {
        typedef basic_ios<T> self;
    public:
        typedef T            value_type;
        typedef size_t       size_type;

        enum radix_type
        {
            binary,
            octonary,
            decimal,
            hexadecimal
        };

        basic_ios() : radix(decimal) {}
    protected:
        template <typename Number>
        bool getInteger(const value_type* p, size_type size, Number& n)
        {
            uchar _base;
            switch (radix)
            {
            case binary:
                _base = 2;
                break;
            case octonary:
                _base = 8;
                break;
            case decimal:
                _base = 10;
                break;
            case hexadecimal:
                _base = 16;
                if (size > 1 && *p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X'))
                {
                    p += 2;
                    size -= 2;
                }
                break;
            }
            bool bNegative = false;
            if (*p == '-' || *p == '+')
            {
                bNegative = (*p == '-');
                if (radix == binary) throw error<string>("binary doesn't support sign", __FILE__, __LINE__);
                ++p;
                --size;
            }
            if (size == 0) return false;

            Number _max = numeric_limits<Number>::max() / _base;

            n = 0;
            while (size)
            {
                uchar ch;
                if (*p >= '0' && *p <= '9') ch = *p - '0';
                else if (*p >= 'a' && *p <= 'f') ch = 10 + *p - 'a';
                else if (*p >= 'A' && *p <= 'F') ch = 10 + *p - 'A';
                else break;

                if (ch >= _base) break;

                n = n * _base + ch;

                if (n >= _max) break;

                --p;
                --size;
            }
            if (bNegative) n = -n;
            return true;
        }

        inline string convert(long l)     { return string::format("%ld", l); }
        inline string convert(ulong ul)   { return string::format("%u", ul); }
        inline string convert(llong ll)   { return string::format("%ld", ll); }
        inline string convert(ullong ull) { return string::format("%u", ull); }
    protected:
        radix_type radix;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
