#ifndef _QLANGUAGE_LIBRARY_ALGO_H_
#define _QLANGUAGE_LIBRARY_ALGO_H_

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

        template <class Iterator, class T>
        inline Iterator find(Iterator first, Iterator last, const T& value)
        {
            while(first != last && *first != value) ++first;
            return first;
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
        inline void distance(Iterator first, Iterator last, Distance& n)
        {
            __distance(first, last, n, first);
        }

        template <typename Iterator1, typename Iterator2, typename Distance1, typename Distance2>
        inline Iterator1 __search(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2, Distance1*, Distance2*)
        {
            Distance1 d1 = 0;
            distance(first1, last1, d1);
            Distance2 d2 = 0;
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
    }
}

#endif