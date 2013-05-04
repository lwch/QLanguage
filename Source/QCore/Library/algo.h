/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:29
	filename: 	\QCore\Library\algo.h
	file path:	\QCore\Library
	file base:	algo
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_ALGO_H_
#define _QLANGUAGE_LIBRARY_ALGO_H_

#include "typedef.h"
#include "iterator.h"

namespace QLanguage
{
    namespace Library
    {
#ifdef min
#undef min
#endif
        template <typename T>
        inline const T& min(const T& a, const T& b)
        {
            return a < b ? a : b;
        }

        template <typename T, typename Compare>
        inline const T& min(const T& a, const T& b, Compare compare)
        {
            return compare(a, b) ? a : b;
        }

#ifdef max
#undef max
#endif
        template <typename T>
        inline const T& max(const T& a, const T& b)
        {
            return a < b ? b : a;
        }

        template <typename T, typename Compare>
        inline const T& max(const T& a, const T& b, Compare compare)
        {
            return compare(a, b) ? b : a;
        }

        template <class Iterator, class Size, class T>
        inline Iterator fill_n(Iterator first, Size count, const T& x)
        {
            while(count--)
            {
                *first = x;
                ++first;
            }
            return first;
        }

        template <class Iterator1, class Iterator2>
        inline Iterator2 copy_backward(Iterator1 first, Iterator1 last, Iterator2 result)
        {
            while(first != last) *--result = *--last;
            return result;
        }

        template <typename Iterator, typename Distance>
        inline void __distance(Iterator first, Iterator last, Distance& n, const bidirectional_iterator&)
        {
            while(first != last) { ++first; ++n; }
        }

        template <typename Iterator, typename Distance>
        inline void __distance(Iterator first, Iterator last, Distance& n, const random_access_iterator&)
        {
            n += last - first;
        }

        template <typename Iterator, typename Distance>
        inline void __distance(Iterator first, Iterator last, Distance& n, const char*)
        {
            __distance(first, last, n, random_access_iterator());
        }

        template <typename Iterator, typename Distance>
        inline void distance(Iterator first, Iterator last, Distance& n)
        {
            __distance(first, last, n, first);
        }

        template <typename Iterator1, typename Iterator2, typename Distance1, typename Distance2>
        inline Iterator1 __search(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2, Distance1*, Distance2*)
        {
            typename remove_const<Distance1>::no_const d1 = 0;
            distance(first1, last1, d1);
            typename remove_const<Distance2>::no_const d2 = 0;
            distance(first2, last2, d2);

            if(d1 < d2) return last1;

            Iterator1 current1 = first1;
            Iterator2 current2 = first2;

            while (current2 != last2)
            {
                if (*current1++ != *current2++)
                {
                    if (d1-- == d2) return last1;
                    else
                    {
                        current1 = ++first1;
                        current2 = first2;
                    }
                }
            }
            return (current2 == last2) ? first1 : last1;
        }

        template <typename Iterator1, typename Iterator2>
        inline Iterator1 search(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2)
        {
            return __search(first1, last1, first2, last2, distance_type(first1), distance_type(first2));
        }

        template <typename T>
        inline void swap(T& a, T& b)
        {
            T tmp = a;
            a = b;
            b = tmp;
        }

        template <typename Iterator>
        inline void iterator_swap(Iterator a, Iterator b)
        {
            typename iterator_traits<Iterator>::value_type v = *a;
            *a = *b;
            *b = v;
        }

        template <typename Iterator, typename Compare>
        void sort(Iterator first, Iterator last, Compare comp)
        {
            if (first == last) return;
            Iterator i = --last;
            Iterator _first = first;
            bool b;
            while (i != first)
            {
                b = false;
                Iterator j = _first;
                ++j;
                for (first = _first; first != i; ++first, ++j)
                {
                    if (!comp(*first, *j))
                    {
                        iterator_swap(first, j);
                        b = true;
                    }
                }
                if (!b) break;
                --i;
            }
        }

        template <typename Iterator>
        void sort(Iterator first, Iterator last)
        {
            if (first == last) return;
            Iterator i = --last;
            Iterator _first = first;
            bool b;
            while (i != first)
            {
                b = false;
                Iterator j = _first;
                ++j;
                for (first = _first; first != i; ++first, ++j)
                {
                    if (!(*first < *j))
                    {
                        iterator_swap(first, j);
                        b = true;
                    }
                }
                if (!b) break;
                --i;
            }
        }

        template <typename Iterator, typename Compare>
        Iterator find(Iterator first, Iterator last, const typename iterator_traits<Iterator>::value_type& value, Compare comp)
        {
            while (first != last)
            {
                if (comp(*first, value)) return first;
                ++first;
            }
            return first;
        }

        template <typename Iterator>
        Iterator find(Iterator first, Iterator last, const typename iterator_traits<Iterator>::value_type& value)
        {
            while (first != last)
            {
                if (*first == value) return first;
                ++first;
            }
            return first;
        }

        template <typename Iterator, typename Value>
        Iterator find(Iterator first, Iterator last, const Value& value)
        {
            while (first != last)
            {
                if (*first == value) return first;
                ++first;
            }
            return first;
        }

        template <typename Iterator, typename Value, typename Compare>
        Iterator find(Iterator first, Iterator last, const Value& value, Compare comp)
        {
            while (first != last)
            {
                if (comp(*first, value)) return first;
                ++first;
            }
            return first;
        }

        template <typename Size, typename Iterator, typename Value>
        Size count(Iterator first, Iterator last, const Value& value)
        {
            Size n = 0;
            while (first != last)
            {
                if (*first == value) ++n;
                ++first;
            }
            return n;
        }

        template <typename Size, typename Iterator, typename Value, typename Compare>
        Size count(Iterator first, Iterator last, const Value& value, Compare comp)
        {
            Size n = 0; 
            while (first != last)
            {
                if (comp(*first, value)) ++n;
                ++first;
            }
            return n;
        }

        template <typename Iterator, typename Value>
        long index_of(Iterator first, Iterator last, const Value& value)
        {
            ulong n = 0;
            while (first != last)
            {
                if (*first == value) return n;
                ++first;
                ++n;
            }
            return -1;
        }

        template <typename Iterator, typename Value, typename Compare>
        long index_of(Iterator first, Iterator last, const Value& value, Compare comp)
        {
            ulong n = 0;
            while (first != last)
            {
                if (comp(*first, value)) return n;
                ++first;
                ++n;
            }
            return -1;
        }

        template <typename Container, typename Compare, typename AddItem>
        void select_into(const Container& container, Container& out, Compare compare, AddItem add)
        {
            for (typename __container_traits<Container>::const_iterator i = container.begin(), m = container.end(); i != m; ++i)
            {
                if (compare(*i)) add(out, *i);
            }
        }

        template <typename Container1, typename Container2, typename Compare, typename AddItem>
        void select_into(const Container1& container, Container2& out, Compare compare, AddItem add)
        {
            for (typename __container_traits<Container1>::const_iterator i = container.begin(), m = container.end(); i != m; ++i)
            {
                if (compare(*i)) add(out, *i);
            }
        }

        template <typename Iterator, typename Value>
        inline void toArray(Iterator first, Iterator last, Value* pArray)
        {
            size_t idx = 0;
            while (first != last) { pArray[idx++] = *first; ++first; }
        }

        template <typename Value, typename Size_Type>
        inline const bool compareArray(const Value* pArray1, Size_Type size1, const Value* pArray2, Size_Type size2)
        {
            if (size1 != size2) return false;

            --size2;
            while (size1--)
            {
                if (pArray1[size2--] != pArray2[size2]) return false;
            }
            return true;
        }
    }
}

#endif