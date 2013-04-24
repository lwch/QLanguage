/********************************************************************
	created:	2013/01/02
	created:	2:1:2013   20:43
	filename: 	\QCore\Library\system.cpp
	file path:	\QCore\Library
	file base:	system
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/

#include "system.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

inline bool isX86()
{
    return sizeof(void*) == 4;
}

extern inline bool isX64()
{
    return sizeof(void*) == 8;
}

inline bool isWindows()
{
#ifdef WIN32
    return true;
#else
    return false;
#endif
}

inline bool isUnix()
{
#ifdef unix
    return true;
#else
    return false;
#endif
}

NAMESPACE_QLANGUAGE_LIBRARY_END