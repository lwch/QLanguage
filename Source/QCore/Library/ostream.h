/********************************************************************
	created:	2013/04/11
	created:	11:4:2013   10:47
	filename: 	\QCore\Library\ostream.h
	file path:	\QCore\Library
	file base:	ostream
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_OSTREAM_H_
#define _QLANGUAGE_LIBRARY_OSTREAM_H_

#include "definition.h"
#include "ios.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    class basic_ostream : public basic_ios<T>
    {
        typedef basic_ostream<T>                self;
        typedef basic_ios<T>                    parent;
        typedef size_t                          size_type;
    public:
        basic_ostream() : parent() {}

        virtual self& operator<<(self& (*f)(self&))=0;
        virtual self& operator<<(bool)=0;
        virtual self& operator<<(short)=0;
        virtual self& operator<<(ushort)=0;
        virtual self& operator<<(int)=0;
        virtual self& operator<<(uint)=0;
        virtual self& operator<<(long)=0;
        virtual self& operator<<(ulong)=0;
        virtual self& operator<<(llong)=0;
        virtual self& operator<<(ullong)=0;
        virtual self& operator<<(float)=0;
        virtual self& operator<<(double)=0;
        virtual self& operator<<(typename basic_ios<T>::_unsigned)=0;
        virtual self& operator<<(typename basic_ios<T>::_signed)=0;
        virtual self& operator<<(typename basic_ios<T>::_unsigned*)=0;
        virtual self& operator<<(const typename basic_ios<T>::_unsigned*)=0;
        virtual self& operator<<(typename basic_ios<T>::_signed*)=0;
        virtual self& operator<<(const typename basic_ios<T>::_signed*)=0;
        virtual self& operator<<(const string&)=0;

        template <typename T1>
        self& operator<<(const T1&)
        {
            throw error<const char*>("doesn't support", __FILE__, __LINE__);
            return *this;
        }

        virtual bool write(const void* buffer, size_type size)=0;
        virtual bool flush()=0;
    protected:
        inline string convert(long l)     { return string::format("%ld", l);  }
        inline string convert(ulong ul)   { return string::format("%u", ul);  }
        inline string convert(llong ll)   { return string::format("%ld", ll); }
        inline string convert(ullong ull) { return string::format("%u", ull); }
        inline string convert(float f)    { return string::format("%f", f);   }
        inline string convert(double d)   { return string::format("%f", d);   }
    };

    typedef basic_ostream<char> ostream;

    template <typename T>
    inline T& endl(T& stream)
    {
#ifdef WIN32
        stream.write("\r\n", 2);
#else
        stream.write("\n", 1);
#endif
        stream.flush();
        return stream;
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
