/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   0:03
	filename: 	\QCore\Library\function_additional.h
	file path:	\QCore\Library
	file base:	function_additional
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_FUNCTION_ADDITIONAL_H_
#define _QLANGUAGE_LIBRARY_FUNCTION_ADDITIONAL_H_

#include "definition.h"
#include "vector.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    void push_back_vector(vector<T>& v, const T& x)
    {
        v.push_back(x);
    }

    template <typename T>
    void push_back_unique_vector(vector<T>& v, const T& x)
    {
        v.push_back_unique(x);
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
