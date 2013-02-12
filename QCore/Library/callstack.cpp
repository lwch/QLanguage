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

#include "error.h"
#include "system.h"
#include "callstack.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

#if defined(_DEBUG) && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)

#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "Psapi.lib" )

bool CallStack::loaded = false;

CallStack::CallStack()
{
    hProcess = GetCurrentProcess();
    DWORD dwOptions = SymGetOptions();
    dwOptions |= SYMOPT_LOAD_LINES;
    dwOptions |= SYMOPT_DEBUG;
    if (!loaded)
    {
        if (SymSetOptions(dwOptions) == 0) throw error<char*>("SymSetOptions failed", __FILE__, __LINE__);
        if (SymInitialize(GetCurrentProcess(), NULL, TRUE) == FALSE) throw error<char*>("SymInitialize failed", __FILE__, __LINE__);
        if (!loadAllModules()) throw error<char*>("loadAllModules failed", __FILE__, __LINE__);
        loaded = true;
    }
}

CallStack::~CallStack()
{
}

DWORD CallStack::stackTrace(UINT_PTR* pCallStack, DWORD dwMaxDepth)
{
    CaptureStackBackTrace(0, dwMaxDepth, (PVOID*)pCallStack, NULL);
    DWORD dwCount = 0;
    while (dwCount < dwMaxDepth)
    {
        if (pCallStack[dwCount] == 0) break;
        ++dwCount;
    }
    return dwCount;
}

bool CallStack::loadAllModules()
{
    const int iMax = 4096;
    HMODULE hModule[iMax] = {0};

    DWORD dwNeeded = 0;
    if (!EnumProcessModules(hProcess, hModule, sizeof(hModule), &dwNeeded)) return false;

    const int iCount = dwNeeded / sizeof(HMODULE);

    for (int i = 0; i < iCount; ++i)
    {
        TCHAR szModuleName[iMax] = {0};
        TCHAR szImageName[iMax]  = {0};

        MODULEINFO info;

        GetModuleInformation(hProcess, hModule[i], &info, sizeof(info));
        GetModuleFileNameEx(hProcess, hModule[i], szImageName, iMax);
        GetModuleBaseName(hProcess, hModule[i], szModuleName, iMax);

        if (isX64()) SymLoadModule64(hProcess, hModule[i], szImageName, szModuleName, (DWORD64)info.lpBaseOfDll, info.SizeOfImage);
        else SymLoadModule(hProcess, hModule[i], szImageName, szModuleName, (DWORD)info.lpBaseOfDll, info.SizeOfImage);
    }
    return true;
}

void CallStack::getFuncInfo(UINT_PTR dwFunc, FuncInfo& info)
{
    memset(szBuffer, 0, sizeof(szBuffer));
    if (isX64())
    {
        PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)szBuffer;
        symbol->SizeOfStruct  = sizeof(szBuffer);
        symbol->MaxNameLength = sizeof(szBuffer) - sizeof(IMAGEHLP_SYMBOL64);

        DWORD64 dwDisplacement = 0;

        if (SymGetSymFromAddr64(hProcess, dwFunc, &dwDisplacement, symbol))
        {
            strcpy_s(info.szFuncName, sizeof(info.szFuncName), symbol->Name);
        }

        IMAGEHLP_LINE64 imageHelpLine;
        imageHelpLine.SizeOfStruct = sizeof(imageHelpLine);

        if (SymGetLineFromAddr64(hProcess, dwFunc, (PDWORD)&dwDisplacement, &imageHelpLine))
        {
            strcpy_s(info.szFilePath, sizeof(info.szFilePath), imageHelpLine.FileName);
            info.dwLineNumber = imageHelpLine.LineNumber;
        }
    }
    else
    {
        PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)szBuffer;
        symbol->SizeOfStruct  = sizeof(szBuffer);
        symbol->MaxNameLength = sizeof(szBuffer) - sizeof(IMAGEHLP_SYMBOL);

        DWORD dwDisplacement = 0;

        if (SymGetSymFromAddr(hProcess, (DWORD)dwFunc, &dwDisplacement, symbol))
        {
            strcpy_s(info.szFuncName, sizeof(info.szFuncName), symbol->Name);
        }

        IMAGEHLP_LINE imageHelpLine;
        imageHelpLine.SizeOfStruct = sizeof(imageHelpLine);

        if (SymGetLineFromAddr(hProcess, (DWORD)dwFunc, &dwDisplacement, &imageHelpLine))
        {
            strcpy_s(info.szFilePath, sizeof(info.szFilePath), imageHelpLine.FileName);
            info.dwLineNumber = imageHelpLine.LineNumber;
        }
    }
}

#endif

NAMESPACE_QLANGUAGE_LIBRARY_END