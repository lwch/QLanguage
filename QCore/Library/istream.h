/********************************************************************
	created:	2013/04/11
	created:	11:4:2013   12:53
	filename: 	\QCore\Library\istream.h
	file path:	\QCore\Library
	file base:	istream
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_ISTREAM_H_
#define _QLANGUAGE_LIBRARY_ISTREAM_H_

#include "definition.h"
#include "ios.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    class basic_istream : public basic_ios<T>
    {
        typedef basic_istream<T> self;
        typedef basic_ios<T>     parent;
    public:
        typedef T      value_type;
        typedef size_t size_type;
        basic_istream() : parent() {}

        virtual self& operator>>(bool&)=0;
        virtual self& operator>>(short&)=0;
        virtual self& operator>>(ushort&)=0;
        virtual self& operator>>(int&)=0;
        virtual self& operator>>(uint&)=0;
        virtual self& operator>>(long&)=0;
        virtual self& operator>>(ulong&)=0;
        virtual self& operator>>(llong&)=0;
        virtual self& operator>>(ullong&)=0;
        virtual self& operator>>(typename parent::_unsigned&)=0;
        virtual self& operator>>(typename parent::_signed&)=0;
        virtual self& operator>>(string&)=0;

        template <typename T1>
        self& operator>>(T1&)
        {
            throw error<const char*>("doesn't support", __FILE__, __LINE__);
            return *this;
        }

        virtual size_type readToBuffer(void* buffer, size_type size)=0;
    protected:
        template <typename Number>
        bool getInteger(const value_type* p, size_type size, Number& n, size_type& step_size)
        {
            uchar _base;
            const value_type* first = p;
            switch (this->radix)
            {
            case parent::binary:
                _base = 2;
                break;
            case parent::octonary:
                _base = 8;
                break;
            case parent::decimal:
                _base = 10;
                break;
            case parent::hexadecimal:
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
                if (this->radix == parent::binary) throw error<const char*>("binary doesn't support sign", __FILE__, __LINE__);
                ++p;
                --size;
            }
            if (size == 0) return false;
#ifdef max
#undef max
#endif
            Number _max = std::numeric_limits<Number>::max() / _base;

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

                ++p;
                --size;

                if (n >= _max) break;
            }
            getNegative(bNegative, n);
            step_size = p - first;
            return true;
        }

        template <typename Number>
        inline void getNegative(bool bNegative, Number& n)
        {
            if (bNegative) n = -n;
        }

#ifdef MSVC
        template <>
#endif
        inline void getNegative(bool bNegative, ushort& n)
        {
            if (bNegative) throw error<const char*>("negative doesn't support", __FILE__, __LINE__);
        }

#ifdef MSVC
        template <>
#endif
        inline void getNegative(bool bNegative, uint& n)
        {
            if (bNegative) throw error<const char*>("negative doesn't support", __FILE__, __LINE__);
        }

#ifdef MSVC
        template <>
#endif
        inline void getNegative(bool bNegative, ulong& n)
        {
            if (bNegative) throw error<const char*>("negative doesn't support", __FILE__, __LINE__);
        }

#ifdef MSVC
        template <>
#endif
        inline void getNegative(bool bNegative, ullong& n)
        {
            if (bNegative) throw error<const char*>("negative doesn't support", __FILE__, __LINE__);
        }
    };

    typedef basic_istream<char> istream;
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
