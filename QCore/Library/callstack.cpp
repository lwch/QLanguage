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

#if defined(_DEBUG) && defined(WIN32)

#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "Psapi.lib" )

bool CallStack::loaded = false;

CallStack::CallStack()
{
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

void CallStack::stackTrace()
{
    CONTEXT context;
    memset(&context, 0, sizeof(context));

    context.ContextFlags = CONTEXT_FULL;

    __asm
    {
        call FakeFuncCall
FakeFuncCall:
        pop eax
        mov context.Eip, eax
        mov context.Ebp, ebp
        mov context.Esp, esp
    }

    const int iMax = 20;
    QWORD stackArray[iMax] = {0};

    stackWalk(stackArray, iMax, &context);

    for (int i = 0; i < iMax && stackArray[i] != 0; ++i)
    {
        getFuncName(stackArray[i]);
    }
}

bool CallStack::loadAllModules()
{
    HANDLE hProcess = GetCurrentProcess();
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

void CallStack::stackWalk(QWORD* pStackArray, DWORD dwMaxDepth, CONTEXT* pContext)
{
    STACKFRAME stackFrame;
    STACKFRAME64 stackFrame64;

    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread  = GetCurrentThread();

    DWORD dwDepth = 0;

    if (isX64()) memset(&stackFrame64, 0, sizeof(stackFrame64));
    else memset(&stackFrame, 0, sizeof(stackFrame));

    bool bSuccessed = true;

    __try
    {
        if (isX64())
        {
            stackFrame64.AddrPC.Offset    = pContext->Eip;
            stackFrame64.AddrPC.Mode      = AddrModeFlat;
            stackFrame64.AddrStack.Offset = pContext->Esp;
            stackFrame64.AddrStack.Mode   = AddrModeFlat;
            stackFrame64.AddrFrame.Offset = pContext->Ebp;
            stackFrame64.AddrFrame.Mode   = AddrModeFlat;

            while (bSuccessed && (dwDepth < dwMaxDepth))
            {
                bSuccessed = StackWalk64(IMAGE_FILE_MACHINE_I386,
                    hProcess,
                    hThread,
                    &stackFrame64,
                    pContext,
                    NULL,
                    SymFunctionTableAccess64,
                    SymGetModuleBase64,
                    NULL) != FALSE;
                pStackArray[dwDepth] = stackFrame64.AddrPC.Offset;
                ++dwDepth;

                if (!bSuccessed) break;
                if (stackFrame64.AddrFrame.Offset == 0) break;
            }
        }
        else
        {
            stackFrame.AddrPC.Offset    = pContext->Eip;
            stackFrame.AddrPC.Mode      = AddrModeFlat;
            stackFrame.AddrStack.Offset = pContext->Esp;
            stackFrame.AddrStack.Mode   = AddrModeFlat;
            stackFrame.AddrFrame.Offset = pContext->Ebp;
            stackFrame.AddrFrame.Mode   = AddrModeFlat;

            while (bSuccessed && (dwDepth < dwMaxDepth))
            {
                bSuccessed = StackWalk(IMAGE_FILE_MACHINE_I386,
                    hProcess,
                    hThread,
                    &stackFrame,
                    pContext,
                    NULL,
                    SymFunctionTableAccess,
                    SymGetModuleBase,
                    NULL) != FALSE;
                pStackArray[dwDepth] = stackFrame.AddrPC.Offset;
                ++dwDepth;

                if (!bSuccessed) break;
                if (stackFrame.AddrFrame.Offset == 0) break;
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

void CallStack::getFuncName(QWORD dwFunc)
{
    const int iMaxNameLength = 4096;
    HANDLE hProcess = GetCurrentProcess();

    if (isX64())
    {
        char szSymbol[iMaxNameLength + sizeof(IMAGEHLP_SYMBOL64)] = {0};
        PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)szSymbol;
        symbol->SizeOfStruct  = sizeof(szSymbol);
        symbol->MaxNameLength = iMaxNameLength;

        DWORD64 dwDisplacement = 0;

        if (SymGetSymFromAddr64(hProcess, dwFunc, &dwDisplacement, symbol)) ;
        else throw error<char*>("Unknown function name", __FILE__, __LINE__);

        IMAGEHLP_LINE64 imageHelpLine;
        imageHelpLine.SizeOfStruct = sizeof(imageHelpLine);

        if (SymGetLineFromAddr64(hProcess, dwFunc, (PDWORD)&dwDisplacement, &imageHelpLine)) ;
    }
    else
    {
        char szSymbol[iMaxNameLength + sizeof(IMAGEHLP_SYMBOL)] = {0};
        PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)szSymbol;
        symbol->SizeOfStruct  = sizeof(szSymbol);
        symbol->MaxNameLength = iMaxNameLength;

        DWORD dwDisplacement = 0;

        if (SymGetSymFromAddr(hProcess, (DWORD)dwFunc, &dwDisplacement, symbol)) ;
        else throw error<char*>("Unknown function name", __FILE__, __LINE__);

        IMAGEHLP_LINE imageHelpLine;
        imageHelpLine.SizeOfStruct = sizeof(imageHelpLine);

        if (SymGetLineFromAddr(hProcess, (DWORD)dwFunc, &dwDisplacement, &imageHelpLine)) ;
    }
}

#endif

NAMESPACE_QLANGUAGE_LIBRARY_END