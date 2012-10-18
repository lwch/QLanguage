#ifndef _QLANGUAGE_LIBRARY_ERROR_H_
#define _QLANGUAGE_LIBRARY_ERROR_H_

#include <stdio.h>

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        class error
        {
        public:
            error(T description, const char* fileName, size_t line)
            {
                this->description = description;
                this->fileName = fileName;
                this->line = line;
            }
        public:
            T           description;
            const char* fileName;
            size_t      line;
        };
    }
}

#endif
