/********************************************************************
	created:	2013/04/28
	created:	28:4:2013   14:54
	filename: 	\QCore\Library\stdstream.h
	file path:	\QCore\Library
	file base:	stdstream
	file ext:	h
	author:		lwch

	purpose:
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_STD_STREAM_H_
#define _QLANGUAGE_LIBRARY_STD_STREAM_H_

#include "definition.h"
#include "istream.h"
#include "ostream.h"
#include "buffer.h"

#ifdef MSVC
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>

#ifdef WIN32
#include <windows.h>
#endif

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
typedef error<const char*> err;
#define CHECK_FILE_OPEN  if (!this->is_open())                     throw err("not opened file", __FILE__, __LINE__)
#define CHECK_IN_MODE    if (!this->is_in())                       throw err("not in mode"    , __FILE__, __LINE__)
#define CHECK_OUT_MODE   if (!this->is_out() && !this->is_error()) throw err("not out mode"   , __FILE__, __LINE__)
#define CHECK_ERROR_MODE if (!this->is_error())                    throw err("not error mode" , __FILE__, __LINE__)

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif
    template <typename T>
    class stdstream_basic;

    template <typename T>
    class stdstream_buffer : public buffer
    {
    public:
        typedef buffer::value_type   value_type;
        typedef buffer::size_type    size_type;
        typedef buffer::_char_traits char_traits;

        stdstream_buffer(stdstream_basic<T>* parent) : parent(parent) {}
    protected:
        stdstream_basic<T>* parent;
    };

    template <typename T>
    class stdstream_basic : public basic_istream<T>
                          , public basic_ostream<T>
    {
        typedef stdstream_basic<T> self;
        typedef basic_istream<T>   parent_i;
        typedef basic_ostream<T>   parent_o;
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
            uninitialized = 0,
            in    = 1,
            out   = 2,
            error = 4,
        };

        enum colortype
        {
            black = 0,
            red   = 1,
            green = 2,
            blue  = 4,
            light = 8,
            yellow = red | green,
            purple = red | blue,
            ching  = green | blue,
            white  = red | green | blue
        };

        stdstream_basic() : parent_i(), parent_o(), buffer_read(this), openMode(uninitialized), bOpen(false), iFile(STDOUT_FILENO), ulTell(0) {}

        stdstream_basic(openmode mode) : parent_i(), parent_o(), buffer_read(this), iFile(STDOUT_FILENO), ulTell(0)
        {
            open(mode);
        }

        virtual ~stdstream_basic() {}

        self& open(openmode mode)
        {
            openMode = mode;
            bOpen    = true;
            switch (mode)
            {
            case in:
                iFile = STDIN_FILENO;
                break;
            case out:
                iFile = STDOUT_FILENO;
                break;
            case error:
                iFile = STDERR_FILENO;
                break;
            default:
                throw err("not support openmode", __FILE__, __LINE__);
                break;
            }
            return *this;
        }

        inline const bool is_open()const               { return bOpen;                  }
        inline const openmode open_mode()const         { return openMode;               }
        inline const bool is_in()const                 { return openMode == in;         }
        inline const bool is_out()const                { return openMode == out;        }
        inline const bool is_error()const              { return openMode == error;      }
        inline const size_type read_cache_size()const  { return buffer_read.size();     }
        inline const value_type* read_pointer()const   { return buffer_read.pointer();  }
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
                if ((size_t)offset > size()) throw err("offset out of range", __FILE__, __LINE__);
                where  = SEEK_SET;
                ulTell = offset;
                break;
            case end:
                if ((size_t)offset > size()) throw err("offset out of range", __FILE__, __LINE__);
                where  = SEEK_END;
                ulTell = size() - offset;
                break;
            case current:
                if (tell() + offset >= size() || tell() + offset < 0) throw err("offset out of range", __FILE__, __LINE__);
                where   = SEEK_CUR;
                ulTell += offset;
                break;
            }

            LSEEK(iFile, offset, where);

            return *this;
        }

        size_type read()
        {
            CHECK_FILE_OPEN;
            CHECK_IN_MODE;

            if (buffer_read.size()) return buffer_read.size();

            char buffer[stdstream_buffer<T>::align] = {0};

            while (true)
            {
                int _read = ::READ(iFile, buffer, stdstream_buffer<T>::align);
                if (_read == -1) continue;
                ulTell += _read;
                if (buffer[_read - 1] == '\n')
                {
                    buffer_read.append(buffer, _read - 1);
                    break;
                }
            }
            return buffer_read.size();
        }

        virtual size_type readToBuffer(void* buffer, size_type sz)
        {
            CHECK_FILE_OPEN;
            CHECK_IN_MODE;

            char* lpBuffer = reinterpret_cast<char*>(buffer);
            size_type _size  = min(sz, size());
            int readen = 0;
            while (true)
            {
                int _read = ::READ(iFile, lpBuffer, (uint)_size);
                if (_read > 0)
                {
                    readen += _read;
                    ulTell += _read;
                    if ((size_type)readen == _size) return _size;
                    else
                    {
                        lpBuffer += _read;
                        _size    -= _read;
                    }
                }
                else
                {
                    throw err("can't read file", __FILE__, __LINE__);
                }
            }
        }

        virtual bool write(const void* buffer, size_type size)
        {
            CHECK_FILE_OPEN;
            CHECK_OUT_MODE;

            if (size == 0) return true;

            const char* lpBuffer = reinterpret_cast<const char*>(buffer);

            while (true)
            {
                int written = ::WRITE(iFile, lpBuffer, (uint)size);
                if (written > 0)
                {
                    ulTell += written;
                    if ((size_type)written == size) return true;
                    else
                    {
                        lpBuffer += written;
                        size     -= written;
                    }
                }
                else
                {
                    throw err("can't write file", __FILE__, __LINE__);
                    return false;
                }
            }
            return true;
        }

        virtual inline bool flush()
        {
            return true;
        }

        void setColor(uchar color)
        {
#ifdef WIN32
            WORD wdColor = 0;
            if (color & red)   wdColor |= FOREGROUND_RED;
            if (color & green) wdColor |= FOREGROUND_GREEN;
            if (color & blue)  wdColor |= FOREGROUND_BLUE;
            if (color & light) wdColor |= FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE), wdColor);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wdColor);
#else
            char cColor = 0;
            if (color & red)   cColor |= 1;
            if (color & green) cColor |= 2;
            if (color & blue)  cColor |= 4;
            //printf("%s\033[%dm", color & light ? "\033[1m" : "", 30 + cColor);
            *this << string::format("%s\033[%dm", color & light ? "\033[1m" : "", 30 + cColor);
#endif
        }

        inline uchar redWith(uchar color)
        {
            return red | color;
        }

        inline uchar greenWith(uchar color)
        {
            return green | color;
        }

        inline uchar blueWith(uchar color)
        {
            return blue | color;
        }

        inline uchar lightWith(uchar color)
        {
            return light | color;
        }
    protected:
        stdstream_buffer<T> buffer_read;
        openmode openMode;
        bool     bOpen;
        int      iFile;
        ulong    ulTell;
    };

    template <typename T>
    class basic_stdstream : public stdstream_basic<T>
    {
        typedef basic_stdstream<T> self;
        typedef stdstream_basic<T> parent;
        typedef basic_istream<T>   parent_i;
        typedef basic_ostream<T>   parent_o;
    public:
        basic_stdstream() : parent() {}
        basic_stdstream(typename parent::openmode mode) : parent(mode) {}

        virtual ~basic_stdstream() {}

        virtual self& operator>>(bool&)
        {
            CHECK_IN_MODE;
            return *this;
        }

        virtual self& operator>>(short& s)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
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
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
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
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
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
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
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
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
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
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
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
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
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
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            size_t step_size;
            this->getInteger(p, this->read_cache_size(), ul, step_size);
            this->step_read_cache(step_size);
            return *this;
        }

        virtual self& operator>>(typename basic_ios<T>::_unsigned& c)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            c = *p;
            this->step_read_cache(1);
            return *this;
        }

        virtual self& operator>>(typename basic_ios<T>::_signed& c)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            c = *p;
            this->step_read_cache(1);
            return *this;
        }

        virtual self& operator>>(string& s)
        {
            CHECK_IN_MODE;
            const typename parent::value_type* p = this->read_pointer();
            if (this->read_cache_size() < stdstream_buffer<T>::half_align)
            {
                this->read();
                p = this->read_pointer();
            }
            s = string(p, this->read_cache_size());
            this->step_read_cache(s.size());
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

        virtual self& operator<<(float f)
        {
            CHECK_OUT_MODE;

            string str = this->convert(f);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(double d)
        {
            CHECK_OUT_MODE;

            string str = this->convert(d);
            this->write(str.c_str(), str.size());
            return *this;
        }

        virtual self& operator<<(typename basic_ios<T>::_unsigned c)
        {
            CHECK_OUT_MODE;

            this->write(&c, sizeof(typename basic_ios<T>::_unsigned));
            return *this;
        }

        virtual self& operator<<(typename basic_ios<T>::_signed c)
        {
            CHECK_OUT_MODE;

            this->write(&c, sizeof(typename basic_ios<T>::_signed));
            return *this;
        }

        virtual self& operator<<(typename basic_ios<T>::_unsigned* p)
        {
            CHECK_OUT_MODE;

            this->write(p, char_traits<typename basic_ios<T>::_unsigned>::length(p));
            return *this;
        }

        virtual self& operator<<(const typename basic_ios<T>::_unsigned* p)
        {
            CHECK_OUT_MODE;

            this->write(p, char_traits<typename basic_ios<T>::_unsigned>::length(p));
            return *this;
        }

        virtual self& operator<<(typename basic_ios<T>::_signed* p)
        {
            CHECK_OUT_MODE;

            this->write(p, char_traits<typename basic_ios<T>::_signed>::length(p));
            return *this;
        }

        virtual self& operator<<(const typename basic_ios<T>::_signed* p)
        {
            CHECK_OUT_MODE;

            this->write(p, char_traits<typename basic_ios<T>::_signed>::length(p));
            return *this;
        }

        virtual self& operator<<(const string& s)
        {
            CHECK_OUT_MODE;

            this->write(s.c_str(), s.size());
            return *this;
        }

        virtual parent_o& operator<<(parent_o& (*f)(parent_o&))
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

    typedef basic_stdstream<char> stdstream;

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

#undef err
#undef CHECK_FILE_OPEN
#undef CHECK_IN_MODE
#undef CHECK_OUT_MODE
#undef CHECK_ERROR_MODE
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
