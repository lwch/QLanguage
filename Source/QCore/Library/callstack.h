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

#ifdef _DEBUG

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

    struct FuncInfo
    {
        char  szFuncName[512];
        char  szFilePath[1024];
        DWORD dwLineNumber;
    };

    DWORD stackTrace(UINT_PTR* pCallStack, DWORD dwMaxDepth);
    void getFuncInfo(UINT_PTR dwFunc, FuncInfo& info);

    static CallStack& getInstance();
protected:
    bool loadAllModules();

    static bool loaded;

    enum { maxBufferLength = 4096 };
    char szBuffer[maxBufferLength];
#ifdef WIN32
    HANDLE hProcess;
#endif
    enum { iMax = 4096 };
#ifdef WIN32
    HMODULE hModule[iMax];
#endif
    char szModuleName[iMax];
    char szImageName[iMax];
};

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif

#endif
