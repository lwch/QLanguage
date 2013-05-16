/********************************************************************
	created:	2013/02/17
	created:	17:2:2013   21:35
	filename: 	\QCoreUnitTest\Units\TestBase.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestBase
	file ext:	cpp
	author:		lwch

	purpose:
*********************************************************************/

#include "TestBase.h"

#ifndef WIN32
#include <unistd.h>
#endif

namespace QLanguage
{
    namespace UnitTest
    {
        SmallObject::SmallObject() : idx(inc()), type(Int)
        {
            data.int_value = 0;
        }

        SmallObject::SmallObject(char value) : idx(inc()), type(Char)
        {
            data.char_value = value;
        }

        SmallObject::SmallObject(short value) : idx(inc()), type(Short)
        {
            data.short_value = value;
        }

        SmallObject::SmallObject(int value) : idx(inc()), type(Int)
        {
            data.int_value = value;
        }

        SmallObject::SmallObject(long value) : idx(inc()), type(Long)
        {
            data.long_value = value;
        }

        SmallObject::SmallObject(uchar value) : idx(inc()), type(UChar)
        {
            data.uchar_value = value;
        }

        SmallObject::SmallObject(ushort value) : idx(inc()), type(UShort)
        {
            data.ushort_value = value;
        }

        SmallObject::SmallObject(uint value) : idx(inc()), type(UInt)
        {
            data.uint_value = value;
        }

        SmallObject::SmallObject(ulong value) : idx(inc()), type(ULong)
        {
            data.ulong_value = value;
        }

        SmallObject::SmallObject(float value) : idx(inc()), type(Float)
        {
            data.float_value = value;
        }

        SmallObject::SmallObject(double value) : idx(inc()), type(Double)
        {
            data.double_value = value;
        }

        SmallObject::SmallObject(void* value) : idx(inc()), type(Pointer)
        {
            data.pointer_value = value;
        }

        SmallObject::SmallObject(const SmallObject& o)
        {
            idx  = o.idx;
            type = o.type;
            data = o.data;
        }

        uint SmallObject::inc()
        {
            static uint i = 0;
            return i++;
        }

        SmallObject& SmallObject::operator=(const SmallObject& o)
        {
            if (&o != this)
            {
                idx = o.idx;
                type = o.type;
                data = o.data;
            }
            return *this;
        }

        const bool SmallObject::operator==(const SmallObject& o)const
        {
            if (idx != o.idx || type != o.type) return false;

            switch (type)
            {
            case Char:
                return data.char_value    == o.data.char_value;
            case Short:
                return data.short_value   == o.data.short_value;
            case Int:
                return data.int_value     == o.data.int_value;
            case Long:
                return data.long_value    == o.data.long_value;
            case UChar:
                return data.uchar_value   == o.data.uchar_value;
            case UShort:
                return data.ushort_value  == o.data.ushort_value;
            case UInt:
                return data.uint_value    == o.data.uint_value;
            case ULong:
                return data.ulong_value   == o.data.ulong_value;
            case Float:
                return data.float_value   == o.data.float_value;
            case Double:
                return data.double_value  == o.data.double_value;
            case Pointer:
                return data.pointer_value == o.data.pointer_value;
            }
            return false;
        }

        const bool SmallObject::operator!=(const SmallObject& o)const
        {
            if (idx == o.idx && type == o.type) return false;

            switch (type)
            {
            case Char:
                return data.char_value    != o.data.char_value;
            case Short:
                return data.short_value   != o.data.short_value;
            case Int:
                return data.int_value     != o.data.int_value;
            case Long:
                return data.long_value    != o.data.long_value;
            case UChar:
                return data.uchar_value   != o.data.uchar_value;
            case UShort:
                return data.ushort_value  != o.data.ushort_value;
            case UInt:
                return data.uint_value    != o.data.uint_value;
            case ULong:
                return data.ulong_value   != o.data.ulong_value;
            case Float:
                return data.float_value   != o.data.float_value;
            case Double:
                return data.double_value  != o.data.double_value;
            case Pointer:
                return data.pointer_value != o.data.pointer_value;
            }
            return false;
        }

        const bool SmallObject::operator<(const SmallObject& o)const
        {
            switch (type)
            {
            case Char:
                return data.char_value    < o.data.char_value;
            case Short:
                return data.short_value   < o.data.short_value;
            case Int:
                return data.int_value     < o.data.int_value;
            case Long:
                return data.long_value    < o.data.long_value;
            case UChar:
                return data.uchar_value   < o.data.uchar_value;
            case UShort:
                return data.ushort_value  < o.data.ushort_value;
            case UInt:
                return data.uint_value    < o.data.uint_value;
            case ULong:
                return data.ulong_value   < o.data.ulong_value;
            case Float:
                return data.float_value   < o.data.float_value;
            case Double:
                return data.double_value  < o.data.double_value;
            case Pointer:
                return data.pointer_value < o.data.pointer_value;
            }
            return false;
        }

        const bool SmallObject::operator>(const SmallObject& o)const
        {
            switch (type)
            {
            case Char:
                return data.char_value    > o.data.char_value;
            case Short:
                return data.short_value   > o.data.short_value;
            case Int:
                return data.int_value     > o.data.int_value;
            case Long:
                return data.long_value    > o.data.long_value;
            case UChar:
                return data.uchar_value   > o.data.uchar_value;
            case UShort:
                return data.ushort_value  > o.data.ushort_value;
            case UInt:
                return data.uint_value    > o.data.uint_value;
            case ULong:
                return data.ulong_value   > o.data.ulong_value;
            case Float:
                return data.float_value   > o.data.float_value;
            case Double:
                return data.double_value  > o.data.double_value;
            case Pointer:
                return data.pointer_value > o.data.pointer_value;
            }
            return false;
        }

        const bool SmallObject::operator<=(const SmallObject& o)const
        {
            switch (type)
            {
            case Char:
                return data.char_value    <= o.data.char_value;
            case Short:
                return data.short_value   <= o.data.short_value;
            case Int:
                return data.int_value     <= o.data.int_value;
            case Long:
                return data.long_value    <= o.data.long_value;
            case UChar:
                return data.uchar_value   <= o.data.uchar_value;
            case UShort:
                return data.ushort_value  <= o.data.ushort_value;
            case UInt:
                return data.uint_value    <= o.data.uint_value;
            case ULong:
                return data.ulong_value   <= o.data.ulong_value;
            case Float:
                return data.float_value   <= o.data.float_value;
            case Double:
                return data.double_value  <= o.data.double_value;
            case Pointer:
                return data.pointer_value <= o.data.pointer_value;
            }
            return false;
        }

        const bool SmallObject::operator>=(const SmallObject& o)const
        {
            switch (type)
            {
            case Char:
                return data.char_value    >= o.data.char_value;
            case Short:
                return data.short_value   >= o.data.short_value;
            case Int:
                return data.int_value     >= o.data.int_value;
            case Long:
                return data.long_value    >= o.data.long_value;
            case UChar:
                return data.uchar_value   >= o.data.uchar_value;
            case UShort:
                return data.ushort_value  >= o.data.ushort_value;
            case UInt:
                return data.uint_value    >= o.data.uint_value;
            case ULong:
                return data.ulong_value   >= o.data.ulong_value;
            case Float:
                return data.float_value   >= o.data.float_value;
            case Double:
                return data.double_value  >= o.data.double_value;
            case Pointer:
                return data.pointer_value >= o.data.pointer_value;
            }
            return false;
        }
    }
}