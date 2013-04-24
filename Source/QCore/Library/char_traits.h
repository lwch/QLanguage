/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:31
	filename: 	\QCore\Library\char_traits.h
	file path:	\QCore\Library
	file base:	char_traits
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_CHAR_TRAITS_H_
#define _QLANGUAGE_LIBRARY_CHAR_TRAITS_H_

#include <memory.h>
#include <string>

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        class __char_traits_base
        {
        public:
            typedef T char_type;

            static void assign(char_type& c1, const char_type& c2)
            {
                c1 = c2;
            }

            static bool eq(const char_type& c1, const char_type& c2)
            {
                return c1 == c2;
            }

            static bool lt(const char_type& c1, const char_type& c2)
            {
                return c1 < c2;
            }

            static int compare(const char_type* s1, const char_type* s2, size_t n)
            {
                for(size_t i = 0; i < n; ++i)
                {
                    if(!eq(s1[i], s2[i])) return lt(s1[i], s2[i]) ? -1 : 1;
                }
                return 0;
            }

            static size_t length(const char_type* s)
            {
                const char_type nullChar = char_type();
                size_t i = 0;
                while(!eq(s[i], nullChar)) ++i;
                return i;
            }

            static char_type* move(char_type* s1, const char_type* s2, size_t n)
            {
                memmove(s1, s2, n * sizeof(char_type));
                return s1;
            }

            static char_type* copy(char_type* s1, const char_type* s2, size_t n)
            {
                memcpy(s1, s2, n * sizeof(char_type));
                return s1;
            } 

            static char_type* assign(char_type* s, size_t n, char_type c)
            {
                for (size_t i = 0; i < n; ++i) s[i] = c;
                return s;
            }

            static char_type eof()
            {
                return char_type();
            }
        };

        template <typename T>
        class char_traits : public __char_traits_base<T>
        {
        };

        template <>
        class char_traits<char> : public __char_traits_base<char>
        {
        public:
            static int compare(const char* s1, const char* s2, size_t n) 
            {
                return memcmp(s1, s2, n);
            }

            static size_t length(const char* s)
            {
                return strlen(s);
            }

            static void assign(char& c1, const char& c2)
            {
                c1 = c2;
            }

            static char* assign(char* s, size_t n, char c)
            {
                memset(s, c, n); return s;
            }

            static char eof()
            {
                return 0;
            }
        };

        template <>
        class char_traits<wchar_t> : public __char_traits_base<wchar_t>
        {
        };
    }
}

#endif