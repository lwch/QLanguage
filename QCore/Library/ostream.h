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
        typedef basic_ostream<T> self;
        typedef basic_ios<T>     parent;
    public:
        basic_ostream() : parent() {}

        virtual self& operator<<(bool)=0;
        virtual self& operator<<(short)=0;
        virtual self& operator<<(ushort)=0;
        virtual self& operator<<(int)=0;
        virtual self& operator<<(uint)=0;
        virtual self& operator<<(long)=0;
        virtual self& operator<<(ulong)=0;
        virtual self& operator<<(llong)=0;
        virtual self& operator<<(ullong)=0;
        virtual self& operator<<(T)=0;
        virtual self& operator<<(const T*)=0;
        virtual self& operator<<(const string&)=0;

        template <typename T1>
        self& operator<<(const T1&)
        {
            throw error<string>("doesn't support", __FILE__, __LINE__);
            return *this;
        }
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
