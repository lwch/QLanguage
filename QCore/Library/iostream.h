/********************************************************************
	created:	2013/04/29
	created:	29:4:2013   18:04
	filename: 	\QCore\Library\iostream.h
	file path:	\QCore\Library
	file base:	iostream
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_IOSTREAM_H_
#define _QLANGUAGE_LIBRARY_IOSTREAM_H_

#include "definition.h"
#include "stdstream.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    extern basic_stdstream<char> cout;
    extern basic_stdstream<char> cin;
    extern basic_stdstream<char> cerr;
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
