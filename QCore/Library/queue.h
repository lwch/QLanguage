/********************************************************************
	created:	2013/02/11
	created:	11:2:2013   19:32
	filename: 	\QCore\Library\queue.h
	file path:	\QCore\Library
	file base:	queue
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_QUEUE_H_
#define _QLANGUAGE_LIBRARY_QUEUE_H_

#include "definition.h"

#include "vector.h"
#include "list.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T, typename Container = list<T> >
    class queue
    {
    public:
        typedef typename Container::value_type             value_type;
        typedef typename Container::pointer                pointer;
        typedef typename Container::reference              reference;
        typedef typename Container::const_reference        const_reference;
        typedef typename Container::size_type              size_type;
        typedef typename Container::distance_type          distance_type;
        typedef typename Container::iterator               iterator;
        typedef typename Container::const_iterator         const_iterator;
        typedef typename Container::reverse_iterator       reverse_iterator;
        typedef typename Container::const_reverse_iterator const_reverse_iterator;

        queue() {}
        ~queue() {}

        inline const bool empty()const
        {
            return container.empty();
        }

        inline const size_type size()const
        {
            return container.size();
        }

        inline const_reference front()const
        {
            return container.front();
        }

        inline reference front()
        {
            return container.front();
        }

        inline const_reference back()const
        {
            return container.back();
        }

        inline reference back()
        {
            return container.back();
        }

        inline void push(const T& x)
        {
            container.push_back(x);
        }

        inline void push_unique(const T& x)
        {
            container.push_back_unique(x);
        }

        inline void pop()
        {
            container.pop_front();
        }

        inline const_reference operator[](size_type n)const
        {
            return container[n];
        }

        inline reference operator[](size_type n)
        {
            return container[n];
        }
    protected:
        Container container;
    };
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
