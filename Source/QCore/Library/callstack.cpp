/********************************************************************
	created:	2013/01/02
	created:	2:1:2013   17:00
	filename: 	\QCore\Library\callstack.cpp
	file path:	\QCore\Library
	file base:	callstack
	file ext:	cpp
	author:		lwch

	purpose:
*********************************************************************/
#include <string.h>

#ifdef unix
#include <execinfo.h>
#endif

#include "algo.h"
#include "error.h"
#include "system.h"
#include "callstack.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

#ifdef _DEBUG

bool CallStack::loaded = false;

CallStack::CallStack()
{
#ifdef WIN32
    hProcess = GetCurrentProcess();
    DWORD dwOptions = SymGetOptions();
    dwOptions |= SYMOPT_LOAD_LINES;
    dwOptions |= SYMOPT_DEBUG;
    if (!loaded)
    {
        if (SymSetOptions(dwOptions) == 0) throw error<const char*>("SymSetOptions failed", __FILE__, __LINE__);
        if (SymInitialize(GetCurrentProcess(), NULL, TRUE) == FALSE) throw error<const char*>("SymInitialize failed", __FILE__, __LINE__);
        if (!loadAllModules()) throw error<const char*>("loadAllModules failed", __FILE__, __LINE__);
        loaded = true;
    }
#endif
}

CallStack::~CallStack()
{
}

DWORD CallStack::stackTrace(UINT_PTR* pCallStack, DWORD dwMaxDepth)
{
#ifdef WIN32
    return CaptureStackBackTrace(0, dwMaxDepth, (PVOID*)pCallStack, NULL);
#else
    return backtrace((PVOID*)pCallStack, dwMaxDepth);
#endif
}

bool CallStack::loadAllModules()
{
#ifdef WIN32
    DWORD dwNeeded = 0;
    if (!EnumProcessModules(hProcess, hModule, sizeof(hModule), &dwNeeded)) return false;

    const int iCount = dwNeeded / sizeof(HMODULE);

    for (int i = 0; i < iCount; ++i)
    {
        MODULEINFO info;

        GetModuleInformation(hProcess, hModule[i], &info, sizeof(info));
        GetModuleFileNameEx(hProcess, hModule[i], szImageName, iMax);
        GetModuleBaseName(hProcess, hModule[i], szModuleName, iMax);

#ifdef X64
        SymLoadModule64(hProcess, hModule[i], szImageName, szModuleName, (DWORD64)info.lpBaseOfDll, info.SizeOfImage);
#else
        SymLoadModule(hProcess, hModule[i], szImageName, szModuleName, (DWORD)info.lpBaseOfDll, info.SizeOfImage);
#endif
    }
#endif
    return true;
}

void CallStack::getFuncInfo(UINT_PTR dwFunc, FuncInfo& info)
{
    memset(szBuffer, 0, sizeof(szBuffer));
#ifdef WIN32
#ifdef X64
    PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)szBuffer;
    symbol->SizeOfStruct  = sizeof(szBuffer);
    symbol->MaxNameLength = sizeof(szBuffer) - sizeof(IMAGEHLP_SYMBOL64);

    DWORD64 dwDisplacement = 0;

    if (SymGetSymFromAddr64(hProcess, dwFunc, &dwDisplacement, symbol))
    {
        strncpy(info.szFuncName, symbol->Name, min(sizeof(info.szFuncName) - 1, strlen(symbol->Name)));
        info.szFuncName[min(sizeof(info.szFuncName) - 1, strlen(symbol->Name))] = 0;
    }

    IMAGEHLP_LINE64 imageHelpLine;
    imageHelpLine.SizeOfStruct = sizeof(imageHelpLine);

    if (SymGetLineFromAddr64(hProcess, dwFunc, (PDWORD)&dwDisplacement, &imageHelpLine))
    {
        strncpy(info.szFilePath, imageHelpLine.FileName, min(sizeof(info.szFilePath) - 1, strlen(imageHelpLine.FileName)));
        info.szFilePath[min(sizeof(info.szFilePath) - 1, strlen(imageHelpLine.FileName))] = 0;
        info.dwLineNumber = imageHelpLine.LineNumber;
    }
#else
    PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)szBuffer;
    symbol->SizeOfStruct  = sizeof(szBuffer);
    symbol->MaxNameLength = sizeof(szBuffer) - sizeof(IMAGEHLP_SYMBOL);

    DWORD dwDisplacement = 0;

    if (SymGetSymFromAddr(hProcess, (DWORD)dwFunc, &dwDisplacement, symbol))
    {
        strncpy(info.szFuncName, symbol->Name, min(sizeof(info.szFuncName) - 1, strlen(symbol->Name)));
        info.szFuncName[min(sizeof(info.szFuncName) - 1, strlen(symbol->Name))] = 0;
    }

    IMAGEHLP_LINE imageHelpLine;
    imageHelpLine.SizeOfStruct = sizeof(imageHelpLine);

    if (SymGetLineFromAddr(hProcess, (DWORD)dwFunc, &dwDisplacement, &imageHelpLine))
    {
        strncpy(info.szFilePath, imageHelpLine.FileName, min(sizeof(info.szFilePath) - 1, strlen(imageHelpLine.FileName)));
        info.szFilePath[min(sizeof(info.szFilePath) - 1, strlen(imageHelpLine.FileName))] = 0;
        info.dwLineNumber = imageHelpLine.LineNumber;
    }
#endif
#endif
}

CallStack& CallStack::getInstance()
{
    static CallStack cs;
    return cs;
}

#endif

NAMESPACE_QLANGUAGE_LIBRARY_END
