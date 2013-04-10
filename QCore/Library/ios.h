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

#include "definition.h"
#include "string.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    class basic_ios
    {
        typedef basic_ios<T> self;
    public:
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
    protected:
        inline string convert(long l)     { return string::format("%ld", l); }
        inline string convert(ulong ul)   { return string::format("%ul", ul); }
        inline string convert(llong ll)   { return string::format("%ld", ll); }
        inline string convert(ullong ull) { return string::format("%ul", ull); }
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
