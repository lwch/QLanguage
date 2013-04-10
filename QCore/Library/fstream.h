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

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "definition.h"
#include "ios.h"
#include "buffer.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

#define CHECK_FILE_OPEN if (!is_open()) throw error<string>("not opened file", __FILE__, __LINE__)

    template <typename T>
    class fstream_basic;

    template <typename T>
    class fstream_buffer : public buffer
    {
    public:
        typedef buffer::value_type   value_type;
        typedef buffer::size_type    size_type;
        typedef buffer::_char_traits char_traits;

        fstream_buffer(fstream_basic<T>* parent) : parent(parent) {}

        enum { commit_size = 1024 };

        inline const bool check_flush()const
        {
            return container.size() >= commit_size;
        }

        inline bool flush()
        {
            return parent->flush();
        }
    protected:
        fstream_basic<T>* parent;
    };

    template <typename T>
    class fstream_basic : public basic_ios<T>
    {
        typedef fstream_basic<T> self;
    public:
        typedef T      value_type;
        typedef size_t size_type;
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

        fstream_basic() : buffer(this), bOpen(false), iFile(0) {}

        fstream_basic(const char* path, uchar mode) : buffer(this), bOpen(false), iFile(0)
        {
            open(path, mode);
        }

        virtual ~fstream_basic()
        {
            if (is_open())
            {
                flush();
                close();
            }
        }

        self& open(const char* path, uchar mode)
        {
            if (is_open()) throw error<string>("file is open", __FILE__, __LINE__);

            int flag = 0;
            switch (mode & (~binary & ~text))
            {
            case out | append:
                flag = _O_WRONLY | _O_CREAT | _O_APPEND;
                break;
            case in:
                flag = _O_RDONLY;
                break;
            case in | out:
                flag = _O_RDWR | _O_TRUNC | _O_CREAT;
                break;
            default:
                throw error<string>("error open mode", __FILE__, __LINE__);
                break;
            }

            iFile = _open(path, flag, S_IREAD | S_IWRITE);
            bOpen = true;

            return *this;
        }

        bool close()
        {
            CHECK_FILE_OPEN;

            bool bResult = false;
            if (iFile)
            {
                bResult = (_close(iFile) == 0);
                if (bResult) bOpen = false;
            }
            return bResult;
        }

        inline const bool is_open()const         { return bOpen; }
        inline const size_type cache_size()const { return buffer.size(); }

        inline const size_type size()const
        {
            CHECK_FILE_OPEN;

            struct _stat s;
            if (_fstat(iFile, &s) == 0) return s.st_size;
            else return 0;
        }

        inline const size_type tell()const
        {
            CHECK_FILE_OPEN;

            return _tell(iFile);
        }

        self& seek(size_type offset, seektype type)
        {
            CHECK_FILE_OPEN;

            int where;

            switch (type)
            {
            case begin:
                if (offset > size()) throw error<string>("offset out of range", __FILE__, __LINE__);
                where = SEEK_SET;
                break;
            case end:
                if (offset > size()) throw error<string>("offset out of range", __FILE__, __LINE__);
                where = SEEK_END;
                break;
            case current:
                if (tell() + offset >= size() || tell() + offset < 0) throw error<string>("offset out of range", __FILE__, __LINE__);
                where = SEEK_CUR;
                break;
            }

            _lseek(iFile, offset, where);

            return *this;
        }

        bool write(const char* buffer, size_type size)
        {
            CHECK_FILE_OPEN;

            bool bResult = this->buffer.append(buffer, size);

            if (bResult && this->buffer.check_flush()) return this->buffer.flush();
            
            return bResult;
        }

        size_type read(char* buffer, size_type size)
        {
            CHECK_FILE_OPEN;

            if (tell() + size >= this->size()) throw error<string>("read out of range", __FILE__, __LINE__);

            return _read(iFile, buffer, size);
        }

        bool flush()
        {
            CHECK_FILE_OPEN;

            size_type size = buffer.size();
            if (size == 0) return true;
            const typename fstream_buffer<T>::value_type* buffer = this->buffer.pointer();
            while (true)
            {
                size_type written = _write(iFile, buffer, size);
                if (written == size)
                {
                    this->buffer.clear();
                    return true;
                }
                else if (written > 0 && written < size)
                {
                    buffer += written;
                    size -= written;
                }
                else
                {
                    this->buffer.clear();
                    throw error<string>("can't write file", __FILE__, __LINE__);
                }
            }
        }
    protected:
        fstream_buffer<T> buffer;
        bool bOpen;
        int  iFile;
    };

    template <typename T>
    class basic_fstream : public fstream_basic<T>
    {
        typedef basic_fstream<T> self;
        typedef fstream_basic<T> parent;
    public:
        basic_fstream() : parent() {}
        basic_fstream(const char* path, uchar mode) : parent(path, mode) {}

        virtual ~basic_fstream() {}

        virtual self& operator<<(bool)
        {
            return *this;
        }

        virtual self& operator<<(short s)
        {
            string str = this->convert(static_cast<long>(s));
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(ushort us)
        {
            string str = this->convert(static_cast<ulong>(us));
            this->write(str.c_str(), str.size());
            return *this;
        }
        
        virtual self& operator<<(int i)
        {
            string str = this->convert(static_cast<long>(i));
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(uint ui)
        {
            string str = this->convert(static_cast<ulong>(ui));
            this->write(str.c_str(), str.size());
            return *this;
        }
        
        virtual self& operator<<(long l)
        {
            string str = this->convert(l);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(ulong ul)
        {
            string str = this->convert(ul);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(llong ll)
        {
            string str = this->convert(ll);
            this->write(str.c_str(), str.size());
            return *this;
        }
        
        virtual self& operator<<(ullong ull)
        {
            string str = this->convert(ull);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(T c)
        {
            this->write(reinterpret_cast<char*>(&c), sizeof(T));
            return *this;
        }

        virtual self& operator<<(const T* p)
        {
            this->write(p, fstream_buffer<T>::char_traits::length(p));
            return *this;
        }

        virtual self& operator<<(const string& s)
        {
            this->write(s.c_str(), s.size());
            return *this;
        }
    };

    typedef basic_fstream<char> fstream;
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
