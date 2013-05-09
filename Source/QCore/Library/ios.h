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
        typedef T                               value_type;
        typedef size_t                          size_type;
        typedef typename unsigned_type<T>::type _unsigned;
        typedef typename signed_type<T>::type   _signed;

        enum radix_type
        {
            binary,
            octonary,
            decimal,
            hexadecimal
        };

        basic_ios() : radix(decimal) {}
    protected:
        radix_type radix;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
