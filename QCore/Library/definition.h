/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:32
	filename: 	\QCore\Library\definition.h
	file path:	\QCore\Library
	file base:	definition
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_DEFINITION_H_
#define _QLANGUAGE_LIBRARY_DEFINITION_H_

#define NAMESPACE_QLANGUAGE_LIBRARY_START namespace QLanguage { namespace Library {
#define NAMESPACE_QLANGUAGE_LIBRARY_END } }

#include "error.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
#define THROW_OUT_OF_RANGE throw error<const char*>("out of range", __FILE__, __LINE__)
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
