#ifndef _QLANGUAGE_LIBRARY_ERROR_H_
#define _QLANGUAGE_LIBRARY_ERROR_H_

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        class error
        {
        public:
            error(T description)
            {
                this->description = description;
            }
        public:
            T description;
        };
    }
}

#endif