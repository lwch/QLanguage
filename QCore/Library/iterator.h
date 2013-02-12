/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:34
	filename: 	\QCore\Library\iterator.h
	file path:	\QCore\Library
	file base:	iterator
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_ITERATOR_H_
#define _QLANGUAGE_LIBRARY_ITERATOR_H_

#include <stddef.h>
#include "traits.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T, typename Size, typename Distance>
        struct __iterator
        {
            typedef T                             value_type;
            typedef Distance                      distance_type;
            typedef T*                            pointer;
            typedef T&                            reference;
            typedef const T*                      const_pointer;
            typedef const T&                      const_reference;
            typedef __iterator<T, Size, Distance> self;
        };

        struct bidirectional_iterator { };
        struct random_access_iterator { };

        template <typename T, typename Size, typename Distance>
        struct __const_iterator
        {
            typedef const T                             value_type;
            typedef const Distance                      distance_type;
            typedef const T*                            pointer;
            typedef const T&                            reference;
            typedef const T*                            const_pointer;
            typedef const T&                            const_reference;
            typedef __const_iterator<T, Size, Distance> self;
        };

        template <typename Iterator, typename T, typename Size = size_t, typename Distance = ptrdiff_t>
        struct __reverse_iterator
        {
            typedef T                                             value_type;
            typedef Distance                                      distance_type;
            typedef T*                                            pointer;
            typedef T&                                            reference;
            typedef const T*                                      const_pointer;
            typedef const T&                                      const_reference;
            typedef __reverse_iterator<Iterator, T, Size, Distance> self;
        protected:
            Iterator pThis;
        public:
            __reverse_iterator()
            {
            }

            __reverse_iterator(Iterator x) : pThis(x)
            {
            }

            template <typename OtherIterator>
            __reverse_iterator(const __reverse_iterator<OtherIterator, T, Size, Distance>& x) : pThis(x.base())
            {
            }

            inline Iterator base()const
            {
                return pThis;
            }

            inline reference operator*()const
            {
                return *(pThis - 1);
            }

            inline self& operator++()
            {
                --pThis;
                return *this;
            }

            inline self operator++(int)
            {
                self tmp = *this;
                --pThis;
                return tmp;
            }

            inline self& operator--()
            {
                ++pThis;
                return *this;
            }

            inline self operator--(int)
            {
                self tmp = *this;
                ++pThis;
                return tmp;
            }

            inline self operator+(Distance count)const
            {
                return self(pThis - count);
            }

            inline self& operator+=(Distance count)
            {
                pThis -= count;
                return *this;
            }

            inline self operator-(Distance count)const
            {
                return self(pThis + count);
            }

            inline self& operator-=(Distance count)
            {
                pThis += count;
                return *this;
            }

            inline bool operator==(const self& x)const
            {
                return pThis == x.pThis;
            }

            inline bool operator!=(const self& x)const
            {
                return pThis != x.pThis;
            }
        };

        template <typename Iterator>
        struct iterator_traits
        {
            typedef typename Iterator::value_type      value_type;
            typedef typename Iterator::distance_type   distance_type;
            typedef typename Iterator::pointer         pointer;
            typedef typename Iterator::reference       reference;
            typedef typename Iterator::const_pointer   const_pointer;
            typedef typename Iterator::const_reference const_reference;
            typedef typename Iterator::self            self_type;
        };

        template <typename T>
        struct iterator_traits<T*>
        {
            typedef T         value_type;
            typedef ptrdiff_t distance_type;
            typedef T*        pointer;
            typedef T&        reference;
            typedef const T*  const_pointer;
            typedef const T&  const_reference;
            typedef T*        self_type;
        };

        template <typename T>
        struct iterator_traits<const T*>
        {
            typedef T         value_type;
            typedef ptrdiff_t distance_type;
            typedef T*        pointer;
            typedef T&        reference;
            typedef const T*  const_pointer;
            typedef const T&  const_reference;
            typedef const T*  self_type;
        };

        template <typename Iterator>
        inline typename iterator_traits<Iterator>::distance_type* distance_type(const Iterator&)
        {
            return static_cast<typename iterator_traits<Iterator>::distance_type*>(0);
        }

        template <typename Iterator>
        inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
        {
            return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
        }

        template <typename InputIterator, typename OutputIterator>
        inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator dest)
        {
            while (first != last)
            {
                *dest = *first;
                dest++;
                first++;
            }
            return dest;
        }
    }
}

#endif
