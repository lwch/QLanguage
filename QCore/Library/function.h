/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:33
	filename: 	\QCore\Library\function.h
	file path:	\QCore\Library
	file base:	function
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_FUNCTION_H_
#define _QLANGUAGE_LIBRARY_FUNCTION_H_

#include "string.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename Arg, typename Result>
        struct unary_function
        {
            typedef Arg           argument_type;
            typedef Result        result_type;
            typedef const Arg&    argument_const_reference;
            typedef const Result& result_const_reference;
        };

        template <typename Arg1, typename Arg2, typename Result>
        struct binary_function
        {
            typedef Arg1          first_argument_type;
            typedef Arg2          second_argument_type;
            typedef Result        result_type;
            typedef const Arg1&   first_argument_const_reference;
            typedef const Arg1&   second_argument_const_reference;
            typedef const Result& result_const_reference;
        };

        template <typename T>
        struct plus : public binary_function<T, T, T>
        {
            typedef binary_function<T, T, T> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x + y;
            }
        };

        template <typename T>
        struct minus : public binary_function<T, T, T>
        {
            typedef binary_function<T, T, T> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x - y;
            }
        };

        template <typename T>
        struct multiplies : public binary_function<T, T, T>
        {
            typedef binary_function<T, T, T> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x * y;
            }
        };

        template <typename T>
        struct divides : public binary_function<T, T, T>
        {
            typedef binary_function<T, T, T> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x / y;
            }
        };

        template <typename T>
        struct modulus : public binary_function<T, T, T>
        {
            typedef binary_function<T, T, T> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x % y;
            }
        };

        template <typename T>
        struct negate : public unary_function<T, T>
        {
            typedef unary_function<T, T> parent;
            inline typename parent::result_type operator()(typename parent::argument_const_reference x)const
            {
                return -x;
            }
        };

        template <typename T>
        struct equal_to : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x == y;
            }
        };

        template <typename T>
        struct not_equal_to : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x != y;
            }
        };

        template <typename T>
        struct greater : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x > y;
            }
        };

        template <typename T>
        struct less : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x < y;
            }
        };

        template <typename T>
        struct greater_equal : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x >= y;
            }
        };

        template <typename T>
        struct less_equal : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x <= y;
            }
        };

        template <typename T>
        struct logical_and : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x && y;
            }
        };

        template <typename T>
        struct logical_or : public binary_function<T, T, bool>
        {
            typedef binary_function<T, T, bool> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference y)const
            {
                return x || y;
            }
        };

        template <typename T>
        struct logical_not : public unary_function<T, bool>
        {
            typedef unary_function<T, bool> parent;
            inline typename parent::result_type operator()(typename parent::argument_const_reference x)const
            {
                return !x;
            }
        };

        template <typename T>
        struct identity : public unary_function<T, const T&>
        {
            typedef unary_function<T, const T&> parent;
            inline typename parent::result_type operator()(typename parent::argument_const_reference x)const
            {
                return x;
            }
        };

        template <typename Pair>
        struct select1st : public unary_function<Pair, const typename Pair::first_type&>
        {
            typedef unary_function<Pair, const typename Pair::first_type&> parent;
            inline typename parent::result_type operator()(typename parent::argument_const_reference x)const
            {
                return x.first;
            }
        };

        template <typename Pair>
        struct select2nd : public unary_function<Pair, const typename Pair::first_type&>
        {
            typedef unary_function<Pair, const typename Pair::first_type&> parent;
            inline typename parent::result_type operator()(typename parent::argument_const_reference x)const
            {
                return x.second;
            }
        };

        template <typename Arg1, typename Arg2>
        struct project1st : public binary_function<Arg1, Arg2, Arg1>
        {
            typedef binary_function<Arg1, Arg2, Arg1> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference x, typename parent::second_argument_const_reference)const
            {
                return x;
            }
        };

        template <typename Arg1, typename Arg2>
        struct project2nd : public binary_function<Arg1, Arg2, Arg2>
        {
            typedef binary_function<Arg1, Arg2, Arg2> parent;
            inline typename parent::result_type operator()(typename parent::first_argument_const_reference, typename parent::second_argument_const_reference y)const
            {
                return y;
            }
        };

        struct emptystring : public unary_function<string, bool>
        {
            typedef unary_function<string, bool> parent;
            inline bool operator()(const string& x)const
            {
                return x.size() == 0;
            }
        };
    }
}

#endif
