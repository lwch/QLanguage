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
#ifdef MSVC
        typedef container_type::size_type  size_type;
#else
        typedef typename container_type::size_type size_type;
#endif
        typedef char_traits<value_type> _char_traits;

        enum { align = 1024 };

        buffer();
        virtual ~buffer();

        value_type* read(size_type size);
        bool append(const value_type* p);
        bool append(const value_type* p, size_type size);
        bool append(const string& s);
        inline value_type* reserve(size_type size);
        
        inline void clear()                                   { container.clear(); }
        inline const size_type size()const                    { return container.size();  }
        inline const bool empty()const                        { return container.empty(); }
        inline const size_type ROUND_UP(size_type bytes)const { return (bytes + align - 1) & ~(align - 1); }
        inline const value_type* pointer()const               { return container.begin(); }
    protected:
        container_type container;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
