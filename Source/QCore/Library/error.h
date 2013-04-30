/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:32
	filename: 	\QCore\Library\error.h
	file path:	\QCore\Library
	file base:	error
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_ERROR_H_
#define _QLANGUAGE_LIBRARY_ERROR_H_

#include "callstack.h"

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        class basic_string;

        typedef basic_string<char> string;

        template <typename T>
        class error
        {
#if defined(_DEBUG) && DEBUG_LEVEL == 3 && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
#define CALLSTACK_MAX_DEPTH 30
            UINT_PTR  callStack[CALLSTACK_MAX_DEPTH];
            DWORD     dwCallStackDepth; // Real depth
#endif
        public:
            error(T description, const char* fileName, size_t line)
            {
                this->description = description;
                this->fileName = fileName;
                this->line = line;
#if defined(_DEBUG) && DEBUG_LEVEL == 3 && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
                dwCallStackDepth = CallStack::getInstance().stackTrace(callStack, CALLSTACK_MAX_DEPTH);
#endif
            }

            inline void print()const
            {
                print_impl(description);
            }
        public:
            T           description;
            const char* fileName;
            size_t      line;
        protected:
            inline void print_impl(char*)const
            {
                print(description);
            }

            inline void print_impl(const char*)const
            {
                print(description);
            }

            inline void print_impl(const string&)const
            {
                print(description.c_str());
            }

            void print(const char* description)const
            {
#ifdef WIN32
                WORD color = FOREGROUND_RED | FOREGROUND_INTENSITY;
                SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE), color);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else
                printf("\033[1m\033[31m");
#endif
                printf("error: %s in file\n", description);
#ifdef WIN32
                printf("%s on line %d\n", fileName, line);
#else
                printf("%s on line %lu\n", fileName, line);
#endif
#if defined(_DEBUG) && DEBUG_LEVEL == 3 && defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
                for (DWORD j = 0; j < dwCallStackDepth; ++j)
                {
                    CallStack::FuncInfo funcInfo;
                    CallStack::getInstance().getFuncInfo(callStack[j], funcInfo);
                    cerr << string::format("Function: %s", funcInfo.szFuncName) << endl;
                    cerr << string::format("File: %s in line %d", funcInfo.szFilePath, funcInfo.dwLineNumber) << endl;
                }
#endif
            }
        };
    }
}

#endif
