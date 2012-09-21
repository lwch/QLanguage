#ifndef _QLANGUAGE_LIBRARY_ITERATOR_H_
#define _QLANGUAGE_LIBRARY_ITERATOR_H_

#include <stddef.h>
#include "traits.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T, typename Size = size_t, typename Difference = ptrdiff_t>
        struct iterator
        {
            typedef T                               value_type;
            typedef Difference                      difference_type;
            typedef T*                              pointer;
            typedef T&                              reference;
            typedef const T*                        const_pointer;
            typedef const T&                        const_reference;
            typedef iterator<T, Size, Difference>   self;
        };

        template <typename T, typename Size = size_t, typename Difference = ptrdiff_t>
        struct const_iterator : public iterator<T>
        {
        };

        template <typename Iterator, typename T, typename Size = size_t, typename Difference = ptrdiff_t>
        struct reverse_iterator
        {
            typedef T                               value_type;
            typedef Difference                      difference_type;
            typedef T*                              pointer;
            typedef T&                              reference;
            typedef const T*                        const_pointer;
            typedef const T&                        const_reference;
            typedef reverse_iterator<Iterator, T, Size, Difference>   self;
        protected:
            Iterator pThis;
        public:
            reverse_iterator()
            {
            }

            reverse_iterator(Iterator x) : pThis(x)
            {
            }

            template <typename OtherIterator>
            reverse_iterator(const reverse_iterator<OtherIterator, T, Size, Difference>& x) : pThis(x.base())
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

            inline self operator+(Difference count)const
            {
                return Difference(pThis - count);
            }

            inline self& operator+=(Difference count)
            {
                pThis -= count;
                return *this;
            }

            inline self operator-(Difference count)const
            {
                return Difference(pThis + count);
            }

            inline self& operator-=(Difference count)
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
            typedef typename Iterator::difference_type difference_type;
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
            typedef ptrdiff_t difference_type;
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
            typedef ptrdiff_t difference_type;
            typedef T*        pointer;
            typedef T&        reference;
            typedef const T*  const_pointer;
            typedef const T&  const_reference;
            typedef const T*  self_type;
        };

        template <typename Iterator>
        inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&)
        {
            return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
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
