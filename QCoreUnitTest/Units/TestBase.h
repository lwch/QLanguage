#ifndef _QCORE_UNIT_TEST_TESTBASE_H_
#define _QCORE_UNIT_TEST_TESTBASE_H_

#include "../../QCore/Library/error.h"
#include "../../QCore/Library/Console.h"

#include <time.h>

namespace QLanguage
{
    using namespace Library;

    namespace UnitTest
    {
        #define TEST_SPEED_INSERT_COUNT 10000

        #define PrintMessage(fmt, ...) \
        Console::SetColor(false, true, false, true); \
        Console::WriteLine(fmt, ##__VA_ARGS__); \
        Console::SetColor(true, true, true, false)
        #define PrintInformation(fmt, ...) \
        Console::SetColor(true, true, true, true); \
        Console::WriteLine(fmt, ##__VA_ARGS__); \
        Console::SetColor(true, true, true, false)
        #define PrintError(fmt, ...) \
        Console::SetColor(true, false, false, true); \
        Console::WriteLine(fmt, ##__VA_ARGS__); \
        Console::SetColor(true, true, true, false)

        #define TIME_START { clock_t _clock_start_ = clock()
        #define SHOW_TIME_COST PrintInformation("time cost: %ld", clock() - _clock_start_); }
        #define SHOW_TIME_COST_SECONDS PrintInformation("time cost: %f seconds", (double)(clock() - _clock_start_) / (double)CLOCKS_PER_SEC); }

        #define TEST_ASSERT(expression, fmt, ...) \
        if(expression) throw error<string>(string::format(fmt, ##__VA_ARGS__), __FILE__, __LINE__);

        #define TEST_CASE(moduleName) \
        extern void Case_##moduleName(); \
        extern int  retCode; \
        class Test##moduleName \
        { \
        public: \
            Test##moduleName() \
            { \
                try \
                { \
                    PrintMessage(#moduleName); \
                    Case_##moduleName(); \
                    PrintInformation(string::format("%s run successed!", #moduleName)); \
                } \
                catch(const char* e) \
                { \
                    PrintError(e); \
                    --retCode; \
                } \
                catch(const error<string>& e) \
                { \
                    PrintError(e.description); \
                    --retCode; \
                } \
            } \
        } Test##moduleName; \
        void Case_##moduleName()
    }
}

#endif
