/********************************************************************
	created:	2013/04/08
	created:	8:4:2013   16:30
	filename: 	\QCore\Library\buffer.h
	file path:	\QCore\Library
	file base:	buffer
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_BUFFER_H_
#define _QLANGUAGE_LIBRARY_BUFFER_H_

#include "definition.h"
#include "vector.h"
#include "char_traits.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    class buffer
    {
    public:
        typedef char               value_type;
        typedef vector<value_type> container_type;
        typedef container_type::size_type  size_type;
        typedef container_type::value_type value_type;
        typedef char_traits<value_type>    char_traits;

        buffer();
        virtual ~buffer();

        value_type* read(size_type size);
        bool append(value_type* p);
        bool append(const string& s);
        
        inline const size_type size()const                    { return container.size();  }
        inline const bool empty()const                        { return container.empty(); }
        inline const size_type ROUND_UP(size_type bytes)const { return (bytes + align - 1) & ~(align - 1); }
    protected:
        container_type container;
        static const size_type align = 1024;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
