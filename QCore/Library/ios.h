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

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    class basic_ios
    {
        typedef basic_ios<T> self;
    public:
        virtual self& operator<<(T)=0;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
