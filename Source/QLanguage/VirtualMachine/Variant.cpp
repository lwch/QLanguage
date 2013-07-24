/********************************************************************
	created:	2013/07/13
	created:	13:7:2013   15:27
	filename: 	\QLanguage\VirtualMachine\Variant.cpp
	file path:	\QLanguage\VirtualMachine
	file base:	Variant
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "Variant.h"

namespace QLanguage
{
    namespace VM
    {
        Variant::Variant()
            : _type(Undefined)
        {
        }

        Variant::Variant(char x)
            : charValue(x)
            , _type(Char)
        {            
        }

        Variant::Variant(uchar x)
            : ucharValue(x)
            , _type(UChar)
        {
        }

        Variant::Variant(short x)
            : shortValue(x)
            , _type(Short)
        {
        }

        Variant::Variant(ushort x)
            : ushortValue(x)
            , _type(UShort)
        {
        }

        Variant::Variant(int x)
            : intValue(x)
            , _type(Int)
        {
        }

        Variant::Variant(uint x)
            : uintValue(x)
            , _type(UInt)
        {
        }

        Variant::Variant(long x)
            : longValue(x)
            , _type(Long)
        {
        }

        Variant::Variant(ulong x)
            : ulongValue(x)
            , _type(ULong)
        {
        }

        Variant::Variant(llong x)
            : llongValue(x)
            , _type(LLong)
        {
        }

        Variant::Variant(ullong x)
            : ullongValue(x)
            , _type(ULLong)
        {
        }

        Variant::Variant(float x)
            : floatValue(x)
            , _type(Float)
        {
        }

        Variant::Variant(double x)
            : doubleValue(x)
            , _type(Double)
        {
        }

        Variant::Variant(const char* x)
            : _type(String)
        {
            initString(x, strlen(x), 0);
        }

        Variant::Variant(const char* x, size_t start, size_t count /* = -1 */)
            : _type(String)
        {
            initString(x, strlen(x), start, count);
        }

        Variant::Variant(const string& x)
            : _type(String)
        {
            initString(x.c_str(), x.size(), 0);
        }

        Variant::Variant(const string& x, size_t start, size_t count /* = -1 */)
            : _type(String)
        {
            initString(x.c_str(), x.size(), start, count);
        }

        Variant::~Variant()
        {
            if (_type == String)
            {
                destruct_range(stringValue.value, stringValue.value + stringValue.length);
                allocator<char>::deallocate(stringValue.value, stringValue.length);
            }
        }

        const char* Variant::toCharPointer()const
        {
            if (_type != String) throw error<const char*>("Can't cast to const char*", __FILE__, __LINE__);
            return stringValue.value;
        }

        const string Variant::toString()const
        {
            if (_type != String) throw error<const char*>("Can't cast to const string&", __FILE__, __LINE__);
            return string(stringValue.value, stringValue.length);
        }

        void Variant::initString(const char* x, size_t size, size_t start, size_t count /* = -1 */)
        {
#ifdef GNUC
            if (count == -1U)
#else
            if (count == -1)
#endif
                stringValue.length = size - start;
            else
            {
                if (count > size - start) THROW_OUT_OF_RANGE;
                stringValue.length = size - start;
            }
            stringValue.value = allocator<char>::allocate(stringValue.length + 1);
            memcpy(stringValue.value, x + start, stringValue.length);
            stringValue.value[stringValue.length] = 0;
        }

        const bool Variant::operator==(const Variant& v)const
        {
            if (_type != v._type) return false;
            switch (_type)
            {
            case Char:
            case UChar:
            case Short:
            case UShort:
            case Int:
            case UInt:
            case Long:
            case ULong:
            case LLong:
            case ULLong:
                return toLLong() == v.toLLong();
            case Float:
            case Double:
                return toDouble() == v.toDouble();
            case String:
                return compareString(v.stringValue.value, v.stringValue.length);
            case Undefined:
                return false;
            }
            return false;
        }

        const bool Variant::compareString(const char* data, size_t len)const
        {
            if (stringValue.length != len) return false;
            for (size_t i = 0; i < stringValue.length; ++i)
            {
                if (stringValue.value[i] != data[i]) return false;
            }
            return true;
        }
    }
}
