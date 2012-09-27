#ifndef _QLANGUAGE_LIBRARY_CONSOLE_H_
#define _QLANGUAGE_LIBRARY_CONSOLE_H_

#include <stdarg.h>
#include "string.h"

#ifdef _WINDOWS
#include <windows.h>
#endif


namespace QLanguage
{
    namespace Library
    {
        class Console
        {
        public:
            static void Write(string s, ...)
            {
                va_list args;

                va_start(args, s);
                vprintf(s.c_str(), args);
                va_end(args);
            }

            static void Write(const string& s, va_list args)
            {
                vprintf(s.c_str(), args);
            }

            static void WriteLine(string s, ...)
            {
                va_list args;

                va_start(args, s);
                Write(s, args);
                va_end(args);
                Write("\r\n");
            }

            #ifdef _WINDOWS
            static void SetColor(bool red, bool green, bool blue, bool light)
            {
                WORD color = 0;
                if(red) color   |= FOREGROUND_RED;
                if(green) color |= FOREGROUND_GREEN;
                if(blue) color  |= FOREGROUND_BLUE;
                if(light) color |= FOREGROUND_INTENSITY;
                SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE), color);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
            }
            #else if defined(_LINUX)
            static void SetColor(bool red, bool green, bool blue, bool light)
            {
                char color = 0;
                if(red)   color |= 1;
                if(green) color |= 2;
                if(blue)  color |= 4;
                printf("%s\033[%dm", light ? "\033[1m" : "", 30 + color);
            }
            #endif
        };
    }
}

#endif
