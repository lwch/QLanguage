/********************************************************************
	created:	2013/01/02
	created:	2:1:2013   20:41
	filename: 	\QCore\Library\system.h
	file path:	\QCore\Library
	file base:	system
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_SYSTEM_H_
#define _QLANGUAGE_LIBRARY_SYSTEM_H_

#include "definition.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

bool isX86();
extern bool isX64();
bool isWindows();
bool isUnix();

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif