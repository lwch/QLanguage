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

#ifdef MSVC
#include <io.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>

#include "definition.h"
#include "istream.h"
#include "ostream.h"
#include "buffer.h"

#ifdef MSVC
#define OPEN  _open
#define CLOSE _close
#define LSEEK _lseek
#define TELL  _tell
#define FSTAT _fstat
#define WRITE _write
#define READ  _read
#define STAT  _stat
#else
#define OPEN  open
#define CLOSE close
#define LSEEK lseek
#define FSTAT fstat
#define WRITE write
#define READ  read
#define STAT  stat
#endif

NAMESPACE_QLANGUAGE_LIBRARY_START

#define CHECK_FILE_OPEN if (!is_open()) throw error<string>("not opened file", __FILE__, __LINE__)
#define CHECK_IN_MODE   if ((this->open_mode() & fstream_basic<T>::in) == 0)  throw error<string>("not in mode", __FILE__, __LINE__)
#define CHECK_OUT_MODE  if ((this->open_mode() & fstream_basic<T>::out) == 0) throw error<string>("not out mode", __FILE__, __LINE__)

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

        inline const bool check_flush()const
        {
            return container.size() >= align;
        }

        inline bool flush()
        {
            return parent->flush();
        }
    protected:
        fstream_basic<T>* parent;
    };

    template <typename T>
    class fstream_basic : public basic_istream<T>,
                          public basic_ostream<T>
    {
        typedef fstream_basic<T> self;
        typedef basic_istream<T> parent_i;
        typedef basic_ostream<T> parent_o;
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
            out    = 4
        };

        fstream_basic() : parent_i(), parent_o(), buffer_write(this), buffer_read(this), bOpen(false), iFile(0) {}

        fstream_basic(const char* path, uchar mode) : parent_i(), parent_o(), buffer_write(this), buffer_read(this), bOpen(false), iFile(0)
        {
            open(path, mode);
        }

        virtual ~fstream_basic()
        {
            if (is_open())
            {
                if (ucOpenMode & out) flush();
                close();
            }
        }

        self& open(const char* path, uchar mode)
        {
            if (is_open()) throw error<string>("file is open", __FILE__, __LINE__);

            int flag = 0;
            switch (mode)
            {
            case out | append:
                flag = O_WRONLY | O_APPEND | O_CREAT;
                break;
            case out:
                flag = O_WRONLY | O_TRUNC | O_CREAT;
                break;
            case in:
                flag = O_RDONLY;
                break;
            case in | out:
                flag = O_RDWR | O_TRUNC | O_CREAT;
                break;
            default:
                throw error<string>("error open mode", __FILE__, __LINE__);
                break;
            }

#ifdef WIN32
            flag |= O_BINARY;
#endif

            iFile = ::OPEN(path, flag, S_IREAD | S_IWRITE);
            bOpen = true;
            ucOpenMode = mode;

            if (mode & append)
            {
                ulTell = size();
                seek(0, end);
            }
            else ulTell = 0;

            return *this;
        }

        bool close()
        {
            CHECK_FILE_OPEN;

            bool bResult = false;
            if (iFile)
            {
                bResult = (::CLOSE(iFile) == 0);
                if (bResult) bOpen = false;
            }
            return bResult;
        }

        inline const uchar open_mode()const            { return ucOpenMode;             }
        inline const bool is_open()const               { return bOpen;                  }
        inline const size_type write_cache_size()const { return buffer_write.size();    }
        inline const size_type read_cache_size()const  { return buffer_read.size();     }
        inline const value_type* write_pointer()const  { return buffer_write.pointer(); }
        inline const value_type* read_pointer()const   { return buffer_read.pointer();  }
        inline void step_write_cache(size_type size)   { buffer_write.step(size);       }
        inline void step_read_cache(size_type size)    { buffer_read.step(size);        }

        inline const size_type size()const
        {
            CHECK_FILE_OPEN;

            struct STAT s;
            if (FSTAT(iFile, &s) == 0) return s.st_size;
            else return 0;
        }

        inline const size_type tell()const
        {
            CHECK_FILE_OPEN;

            return ulTell;
        }

        self& seek(int offset, seektype type)
        {
            CHECK_FILE_OPEN;

            int where;

            switch (type)
            {
            case begin:
                if ((size_t)offset > size()) throw error<string>("offset out of range", __FILE__, __LINE__);
                where  = SEEK_SET;
                ulTell = offset;
                break;
            case end:
                if ((size_t)offset > size()) throw error<string>("offset out of range", __FILE__, __LINE__);
                where  = SEEK_END;
                ulTell = size() - offset;
                break;
            case current:
                if (tell() + offset >= size() || tell() + offset < 0) throw error<string>("offset out of range", __FILE__, __LINE__);
                where   = SEEK_CUR;
                ulTell += offset;
                break;
            }

            LSEEK(iFile, offset, where);

            return *this;
        }

        bool write(const char* buffer, size_type size)
        {
            CHECK_FILE_OPEN;
            CHECK_OUT_MODE;

            bool bResult = this->buffer_write.append(buffer, size);

            if (bResult && this->buffer_write.check_flush())
            {
                bResult = this->buffer_write.flush();
            }

            return bResult;
        }

        size_type read()
        {
            CHECK_FILE_OPEN;
            CHECK_IN_MODE;

            size_type _size = min(size() - tell(), static_cast<size_type>(fstream_buffer<T>::align));
            value_type* buffer = this->buffer_read.reserve(_size);

            size_type readen = 0;
            while (true)
            {
                size_type _read = ::READ(iFile, buffer, _size);
                readen += _read;
                ulTell += readen;
                if (readen == _size) return _size;
                else if (_read > 0)
                {
                    buffer += _read;
                    _size  -= _read;
                }
                else
                {
                    this->buffer_read.clear();
                    throw error<string>("can't read file", __FILE__, __LINE__);
                }
            }
        }

        bool readAll(value_type* ptr, size_type sz)
        {
            CHECK_FILE_OPEN;
            CHECK_IN_MODE;

            size_type _size  = min(sz, size());
            size_type readen = 0;
            while (true)
            {
                size_type _read = ::READ(iFile, ptr, _size);
                readen += _read;
                ulTell += _read;
                if (readen == _size) return true;
                else if (_read > 0)
                {
                    ptr   += _read;
                    _size -= _read;
                }
                else
                {
                    throw error<string>("can't read file", __FILE__, __LINE__);
                }
            }
        }

        bool flush()
        {
            CHECK_FILE_OPEN;
            CHECK_OUT_MODE;

            size_type size = buffer_write.size();
            if (size == 0) return true;
            const typename fstream_buffer<T>::value_type* buffer = this->buffer_write.pointer();
            while (true)
            {
                size_type written = ::WRITE(iFile, buffer, size);
                ulTell += written;
                if (written == size)
                {
                    this->buffer_write.clear();
                    return true;
                }
                else if (written > 0)
                {
                    buffer += written;
                    size -= written;
                }
                else
                {
                    this->buffer_write.clear();
                    throw error<string>("can't write file", __FILE__, __LINE__);
                }
            }
        }
    protected:
        fstream_buffer<T> buffer_write;
        fstream_buffer<T> buffer_read;
        bool  bOpen;
        int   iFile;
        uchar ucOpenMode;
        ulong ulTell;
    };

    template <typename T>
    class basic_fstream : public fstream_basic<T>
    {
        typedef basic_fstream<T> self;
        typedef fstream_basic<T> parent;
    public:
        basic_fstream() : parent() {}
        basic_fstream(const char* path, uchar mode) : parent(path, mode) {}
        basic_fstream(const string& path, uchar mode) : parent(path.c_str(), mode) {}

        virtual ~basic_fstream() {}

        virtual self& operator>>(bool&)
        {
            CHECK_IN_MODE;
            return *this;
        }

        virtual self& operator>>(short& s)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), s, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(ushort& us)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), us, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(int& i)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), i, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(uint& ui)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), ui, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(long& l)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), l, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(ulong& ul)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), ul, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(llong& ll)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), ll, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(ullong& ul)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), ul, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(T& c)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < fstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            c = *p;
            this->step_read_cache(1);
            return *this;
        }

        template <typename T1>
        self& operator>>(T1& c)
        {
            CHECK_IN_MODE;

            c.create(this->read_pointer(), min(this->read_pointer(), c.file_size()));
            return *this;
        }

        virtual self& operator<<(bool b)
        {
            CHECK_OUT_MODE;

            string str = this->convert(static_cast<ulong>(b));
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(short s)
        {
            CHECK_OUT_MODE;

            string str = this->convert(static_cast<long>(s));
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(ushort us)
        {
            CHECK_OUT_MODE;

            string str = this->convert(static_cast<ulong>(us));
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(int i)
        {
            CHECK_OUT_MODE;

            string str = this->convert(static_cast<long>(i));
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(uint ui)
        {
            CHECK_OUT_MODE;

            string str = this->convert(static_cast<ulong>(ui));
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(long l)
        {
            CHECK_OUT_MODE;

            string str = this->convert(l);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(ulong ul)
        {
            CHECK_OUT_MODE;

            string str = this->convert(ul);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(llong ll)
        {
            CHECK_OUT_MODE;

            string str = this->convert(ll);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(ullong ull)
        {
            CHECK_OUT_MODE;

            string str = this->convert(ull);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(T c)
        {
            CHECK_OUT_MODE;

            this->write(reinterpret_cast<char*>(&c), sizeof(T));
            return *this;
        }

        virtual self& operator<<(const T* p)
        {
            CHECK_OUT_MODE;

            this->write(p, fstream_buffer<T>::char_traits::length(p));
            return *this;
        }

        virtual self& operator<<(const string& s)
        {
            CHECK_OUT_MODE;

            this->write(s.c_str(), s.size());
            return *this;
        }

        self& operator<<(self& (*f)(self&))
        {
            return f(*this);
        }

        template <typename T1>
        self& operator<<(const T1& c)
        {
            CHECK_OUT_MODE;

            this->write(c.data(), c.size());
            return *this;
        }
    };

    typedef basic_fstream<char> fstream;

    template <typename T>
    inline basic_fstream<T>& endl(basic_fstream<T>& fs)
    {
        fs.write("\n", 1);
        fs.flush();
        return fs;
    }

#undef OPEN 
#undef CLOSE
#undef LSEEK
#undef FSTAT
#undef WRITE
#undef READ
#undef STAT

#ifdef MSVC
#undef TELL
#endif

#undef CHECK_FILE_OPEN
#undef CHECK_IN_MODE
#undef CHECK_OUT_MODE
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
