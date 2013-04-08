/********************************************************************
	created:	2013/04/08
	created:	8:4:2013   16:30
	filename: 	\QCore\Library\fstream.h
	file path:	\QCore\Library
	file base:	fstream
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_FSTREAM_H_
#define _QLANGUAGE_LIBRARY_FSTREAM_H_

#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#include "definition.h"
#include "ios.h"
#include "buffer.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    template <typename T>
    class basic_fstream : public basic_ios<T>
    {
        typedef basic_fstream<T> self;
    public:
        typedef T value_type;
    public:
        enum seektype
        {
            begin,
            current,
            end
        };

        enum openmode
        {
            append = 1,
            in     = 2,
            out    = 4,
            binary = 8,
            text   = 16
        };

        basic_fstream() : bOpen(false), iFile(0) {}

        explicit basic_fstream(const char* path, uchar mode)
        {
            int flag = 0;
            switch (mode & (~append & ~binary & ~text))
            {
            case out | append:
                flag = _O_WRONLY | _O_CREAT | _O_APPEND;
                break;
            case in:
                flag = _O_RDONLY;
                break;
            case in | out:
                flag = _O_RDWR;
                break;
            default:
                throw error<string>("error open mode", __FILE__, __LINE__);
                break;
            }

            iFile = _open(path, flag);
        }

        ~basic_fstream()
        {
            if (iFile) _close(iFile);
        }

        virtual self& operator<<(T c)
        {
            return *this;
        }

        inline const bool is_open()const { return bOpen; }
    protected:
        buffer _buffer;
        bool   bOpen;

        int    iFile;
    };

    typedef basic_fstream<char> fstream;
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
