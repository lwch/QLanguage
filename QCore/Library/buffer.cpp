/********************************************************************
	created:	2013/04/08
	created:	8:4:2013   16:45
	filename: 	\QCore\Library\buffer.cpp
	file path:	\QCore\Library
	file base:	buffer
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "string.h"

#include "buffer.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    buffer::buffer()
    {
    }

    buffer::~buffer()
    {
    }

    buffer::value_type* buffer::read(size_type size)
    {
        if (container.size() < size) throw error<string>("read to more", __FILE__, __LINE__);
        return container.begin();
    }

    bool buffer::append(value_type* p)
    {
        size_t size = ROUND_UP(char_traits::length(p));
        container.reserve(container.size() + size);
        for (value_type* q = p + size; p < q; ++p)
        {
            container.push_back(*p);
        }
        return true;
    }

    bool buffer::append(const string& s)
    {
        size_t size = ROUND_UP(s.size());
        container.reserve(container.size() + size);
        for (string::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
        {
            container.push_back(*i);
        }
        return true;
    }
NAMESPACE_QLANGUAGE_LIBRARY_END
