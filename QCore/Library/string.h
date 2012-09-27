#ifndef _QLANGUAGE_LIBRARY_STRING_H_
#define _QLANGUAGE_LIBRARY_STRING_H_

#include "memory.h"
#include "vector.h"
#include "char_traits.h"
#include "function.h"
#include "algo.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        class basic_string : public vector<T>
        {
        protected:
            #define MAX_STRING_RESERVE_SIZE 512
            typedef vector<T> parent;
            typedef char_traits<T> Traits;

            friend class vector<T>;

            static const typename parent::size_type npos = -1;
        public:
            basic_string() : parent(8)
            {
            }

            basic_string(typename parent::size_type n) : parent((n + 1) * 2 <= MAX_STRING_RESERVE_SIZE ? (n + 1) * 2 : n + 1)
            {
            }

            basic_string(const T* s) : parent()
            {
                typename parent::size_type n = Traits::length(s);
                typename parent::size_type _1 = n + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(s, s + n, begin());
                parent::finish = parent::start + n;
                *parent::finish = Traits::eof();
            }

            basic_string(const T* s, typename parent::size_type n) : parent()
            {
                const typename parent::size_type sSize = Traits::length(s);
                if(n > sSize) throw "out of range";
                typename parent::size_type _1 = n + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(s, s + n, begin());
                parent::finish = parent::start + n;
                *parent::finish = Traits::eof();
            }

            basic_string(const basic_string<T>& s)
            {
                const typename parent::size_type n = s.size();
                typename parent::size_type _1 = n + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(s.begin(), s.begin() + n, begin());
                parent::finish = parent::start + n;
                *parent::finish = Traits::eof();
            }

            basic_string(const basic_string<T>& s, typename parent::size_type pos, typename parent::size_type n = npos) : parent()
            {
                const typename parent::size_type sSize = s.size();
                if(pos > sSize || pos < 0) throw "out of range";
                const typename parent::size_type nLength = n == npos ? sSize - pos : min(n, sSize - pos);
                typename parent::size_type _1 = nLength + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(s.begin() + pos, s.begin() + pos + nLength, begin());
                parent::finish = parent::start + nLength;
                *parent::finish = Traits::eof();
            }

            basic_string(const typename parent::size_type& n, const T& c) : parent(n, c)
            {
            }

            template <typename Iterator>
            basic_string(Iterator first, Iterator last) : parent()
            {
                const typename parent::size_type n = last - first;
                typename parent::size_type _1 = n + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(first, last, begin());
                parent::finish = parent::start + n;
                *parent::finish = Traits::eof();
            }

            basic_string(const T* first, const T* last) : parent()
            {
                const typename parent::size_type n = last - first;
                typename parent::size_type _1 = n + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(first, last, begin());
                parent::finish = parent::start + n;
                *parent::finish = Traits::eof();
            }

            ~basic_string()
            {
            }

            inline const T* c_str()const
            {
                return parent::start;
            }

            inline const T* data()const
            {
                return parent::start;
            }

            typename parent::self& append(const basic_string<T>& s)
            {
                return append(s.begin(), s.end());
            }

            typename parent::self& append(const basic_string<T>& s, typename parent::size_type pos, typename parent::size_type n)
            {
                const typename parent::size_type sSize = s.size();
                if(pos > sSize || pos < 0) throw "out of range";
                const typename parent::size_type nLength = n == npos ? sSize - pos : min(n, sSize - pos);
                return append(s.begin(), s.begin() + nLength);
            }

            typename parent::self& append(const T* s)
            {
                return append(s, s + Traits::length(s));
            }

            typename parent::self& append(const T* s, typename parent::size_type pos, typename parent::size_type n)
            {
                const typename parent::size_type sSize = s.size();
                if(pos > sSize || pos < 0) throw "out of range";
                const typename parent::size_type nLength = n == npos ? sSize - pos : min(n, sSize - pos);
                return append(s, s + nLength);
            }

            template <typename Iterator>
            typename parent::self& append(Iterator first, Iterator last)
            {
                const typename parent::size_type newSize = size() + last - first;
                typename parent::size_type _1 = newSize + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(first, last, end());
                parent::finish = parent::start + newSize;
                *parent::finish = Traits::eof();
                return *this;
            }

            typename parent::self& append(const T* first, const T* last)
            {
                const typename parent::size_type newSize = size() + last - first;
                typename parent::size_type _1 = newSize + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(first, last, end());
                parent::finish = parent::start + newSize;
                *parent::finish = Traits::eof();
                return *this;
            }

            typename parent::self& assign(const basic_string<T>& s)
            {
                return assign(s.begin(), s.end());
            }

            typename parent::self& assign(const basic_string<T>& s, typename parent::size_type pos, typename parent::size_type n)
            {
                const typename parent::size_type sSize = s.size();
                if(pos > sSize || pos < 0) throw "out of range";
                const typename parent::size_type nLength = n == npos ? sSize - pos : min(n, sSize - pos);
                return assign(s.begin(), s.begin() + nLength);
            }

            typename parent::self& assign(const T* s)
            {
                return assign(s, s + Traits::length(s));
            }

            typename parent::self& assign(const T* s, typename parent::size_type pos, typename parent::size_type n)
            {
                const typename parent::size_type sSize = s.size();
                if(pos > sSize || pos < 0) throw "out of range";
                const typename parent::size_type nLength = n == npos ? sSize - pos : min(n, sSize - pos);
                return assign(s, s + nLength);
            }

            template <typename Iterator>
            typename parent::self& assign(Iterator first, Iterator last)
            {
                const typename parent::size_type newSize = last - first;
                typename parent::size_type _1 = newSize + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(first, last, begin());
                parent::finish = parent::start + newSize;
                *parent::finish = Traits::eof();
                return *this;
            }

            typename parent::self& assign(const T* first, const T* last)
            {
                const typename parent::size_type newSize = last - first;
                typename parent::size_type _1 = newSize + 1;
                typename parent::size_type _2 = _1 * 2;
                reserve(_2 <= MAX_STRING_RESERVE_SIZE ? _2 : _1);
                copy(first, last, begin());
                parent::finish = parent::start + newSize;
                *parent::finish = Traits::eof();
                return *this;
            }

            const typename parent::size_type find(const basic_string<T>& s, typename parent::size_type pos = 0)const
            {
                return find(s.begin(), pos, s.size());
            }

            const typename parent::size_type find(const T* s, typename parent::size_type pos = 0)const
            {
                return find(s, pos, Traits::length(s));
            }

            template <typename Iterator>
            const typename parent::size_type find(Iterator first, typename parent::size_type pos, typename parent::size_type n)const
            {
                if(pos + n > size()) return npos;
                const typename parent::const_iterator result = search(begin() + pos, end(), first, first + n);
                return result != end() ? result - begin() : npos;
            }

            const typename parent::size_type find(const T* first, typename parent::size_type pos, typename parent::size_type n)const
            {
                if(pos + n > size()) return npos;
                const typename parent::const_iterator result = search(begin() + pos, end(), first, first + n);
                return result != end() ? result - begin() : npos;
            }

            template <typename T2>
            static const basic_string<T2> format(const T* fmt, ...)
            {
                throw error<char*>("Doesn't support!");
                return basic_string<T2>();
            }

            static const basic_string<char> format(const char* fmt, ...)
            {
                va_list l;
                va_start(l, fmt);
                basic_string<T> result;
                int len = 0;
                #ifdef _WINDOWS
                len = _vscprintf(fmt, l);
                result.reserve(len + 1);
                vsprintf(result.begin(), fmt, l);
                result.finish = result.begin() + len;
                #else if defined(_LINUX)
                char* res = NULL;
                len = vasprintf(&res, fmt, l);
                result.start = res;
                result.finish = result.begin() + len;
                result.end_of_element = result.finish;
                #endif
                result.back() = char_traits<char>::eof();
                va_end(l);
                return result;
            }

            static const basic_string<wchar_t> format(const wchar_t* fmt, ...)
            {
                va_list l;
                va_start(l, fmt);
                basic_string<T> result;
                int len = 0;
                #ifdef _WINDOWS
                len = _vscwprintf(fmt, l);
                result.reserve(len + 1);
                vswprintf(result.begin(), fmt, l);
                result.finish = result.begin() + len;
                #else if defined(_LINUX)
                throw error<const wchar_t*>(L"doesn't support");
                #endif
                result.back() = char_traits<wchar_t>::eof();
                va_end(l);
                return result;
            }

            typename parent::self& operator+=(const basic_string<T>& s)
            {
                return append(s);
            }

            typename parent::self& operator+=(const T* s)
            {
                return append(s);
            }

            typename parent::self& operator+=(const T& c)
            {
                parent::push_back(c);
                return *this;
            }

            typename parent::self& operator=(const basic_string<T>& s)
            {
                if(this != &s) assign(s.begin(), s.end());
                return *this;
            }

            typename parent::self& operator=(const T* s)
            {
                return assign(s, s + Traits::length(s));
            }

            typename parent::self& operator=(T c)
            {
                return assign(&c, &c + 1);
            }
        };

        typedef basic_string<char> string;
    }
}

#endif
