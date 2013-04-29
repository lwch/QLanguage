/********************************************************************
	created:	2013/04/29
	created:	29:4:2013   18:06
	filename: 	\QCore\Library\iostream.cpp
	file path:	\QCore\Library
	file base:	iostream
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "iostream.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    basic_stdstream<char> cout(basic_stdstream<char>::out);
    basic_stdstream<char> cin(basic_stdstream<char>::in);
    basic_stdstream<char> cerr(basic_stdstream<char>::error);
NAMESPACE_QLANGUAGE_LIBRARY_END
