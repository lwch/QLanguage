/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:35
	filename: 	\QCore\Library\pair.h
	file path:	\QCore\Library
	file base:	pair
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_PAIR_H_
#define _QLANGUAGE_LIBRARY_PAIR_H_

namespace QLanguage
{
    namespace Library
    {
        template <typename T1, typename T2>
        struct pair
        {
            typedef T1 first_type;
            typedef T2 second_type;

            T1 first;
            T2 second;

            pair() : first(T1()), second(T2())
            {
            }

            pair(const T1& _first, const T2& _second) : first(_first), second(_second)
            {
            }

            pair(const pair<T1, T2>& p) : first(p.first), second(p.second)
            {
            }

            inline const bool operator<(const pair<T1, T2>& x)const
            {
                return first < x.first && second < x.second;
            }

            inline const bool operator==(const pair<T1, T2>& x)const
            {
                return first == x.first && second == x.second;
            }
        };
    }
}

#endif