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
        virtual self& operator>>(T&)=0;

        template <typename T1>
        self& operator>>(T1&)
        {
            throw error<string>("doesn't support", __FILE__, __LINE__);
            return *this;
        }
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
