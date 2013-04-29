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
        if (container.size() < size) throw error<const char*>("read to more", __FILE__, __LINE__);
        return container.begin();
    }

    bool buffer::append(const value_type* p)
    {
        size_t length = _char_traits::length(p);
        if (container.capacity() - container.size() < length)
        {
            size_t size = ROUND_UP(length);
            container.reserve(container.size() + size);
        }
        for (const value_type* q = p + length; p < q; ++p)
        {
            container.push_back(*p);
        }
        return true;
    }

    bool buffer::append(const value_type* p, size_type size)
    {
        size_t length = size;
        if (container.capacity() - container.size() < length)
        {
            size = ROUND_UP(length);
            container.reserve(container.size() + size);
        }
        for (const value_type* q = p + length; p < q; ++p)
        {
            container.push_back(*p);
        }
        return true;
    }

    bool buffer::append(const string& s)
    {
        if (container.capacity() - container.size() < s.size())
        {
            size_t size = ROUND_UP(s.size());
            container.reserve(container.size() + size);
        }
        for (string::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
        {
            container.push_back(*i);
        }
        return true;
    }

    void buffer::step(size_type size)
    {
        if (size > container.size()) throw error<const char*>("step size too big", __FILE__, __LINE__);

        copy(container.begin() + size, container.end(), container.begin());
        destruct(container.end() - size, container.end());
        container.setsize(container.size() - size);
    }
NAMESPACE_QLANGUAGE_LIBRARY_END
