/********************************************************************
	created:	2013/01/02
	created:	2:1:2013   15:55
	filename: 	\QCore\Library\callstack.h
	file path:	\QCore\Library
	file base:	callstack
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_CALLSTACK_H_
#define _QLANGUAGE_LIBRARY_CALLSTACK_H_

#if defined(_DEBUG) && defined(WIN32)

#include <windows.h>
#include <WinDNS.h>
#include <DbgHelp.h>
#include <Psapi.h>

#include "definition.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

class CallStack
{
public:
    CallStack();
    ~CallStack();

    void stackTrace();
protected:
    bool loadAllModules();
    void stackWalk(QWORD* pStackArray, DWORD dwMaxDepth, CONTEXT* pContext);
    void getFuncName(QWORD dwFunc);

    static bool loaded;
};

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif

#endif
