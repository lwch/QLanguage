#ifndef _QCORE_UNIT_TEST_TESTBASE_H_
#define _QCORE_UNIT_TEST_TESTBASE_H_

#include "../../QCore/Library/error.h"
#include "../../QCore/Library/Console.h"

namespace QLanguage
{
    using namespace Library;

    namespace UnitTest
    {
        #define PrintMessage(fmt, ...) \
        Console::SetColor(false, true, false, true); \
        Console::WriteLine(fmt, ##__VA_ARGS__); \
        Console::SetColor(true, true, true, false);
        #define PrintInformation(fmt, ...) \
        Console::SetColor(true, true, true, true); \
        Console::WriteLine(fmt, ##__VA_ARGS__); \
        Console::SetColor(true, true, true, false);
        #define PrintError(fmt, ...) \
        Console::SetColor(true, false, false, true); \
        Console::WriteLine(fmt, ##__VA_ARGS__); \
        Console::SetColor(true, true, true, false);

        #define TEST_ASSERT(expression, fmt, ...) \
        if(expression) throw error<string>(string::format(fmt, ##__VA_ARGS__));

        #define TEST_CASE(moduleName) \
        extern void Case_##moduleName(); \
        class Test##moduleName \
        { \
        public: \
            Test##moduleName() \
            { \
                PrintMessage(#moduleName); \
                Case_##moduleName(); \
                PrintInformation(string::format("%s run successed!", #moduleName)); \
            } \
        } Test##moduleName; \
        void Case_##moduleName()
    }
}

#endif
