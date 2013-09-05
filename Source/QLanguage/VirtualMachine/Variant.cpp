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

/* CHECK2_2* */
#define CHECK2_2(op, v1, v2) \
        do \
        { \
            switch (v2.type()) \
            { \
            case Char: \
                return Variant(v1 op v2.toChar()); \
            case UChar: \
                return Variant(v1 op v2.toUChar()); \
            case Short: \
                return Variant(v1 op v2.toShort()); \
            case UShort: \
                return Variant(v1 op v2.toUShort()); \
            case Int: \
                return Variant(v1 op v2.toInt()); \
            case UInt: \
                return Variant(v1 op v2.toUInt()); \
            case Long: \
                return Variant(v1 op v2.toLong()); \
            case ULong: \
                return Variant(v1 op v2.toULong()); \
            case LLong: \
                return Variant(v1 op v2.toLLong()); \
            case ULLong: \
                return Variant(v1 op v2.toULLong()); \
            case Float: \
                return Variant(v1 op v2.toFloat()); \
            case Double: \
                return Variant(v1 op v2.toDouble()); \
            case String: \
                break; \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_2_SIGNED(op, v1, v2) \
        do \
        { \
            switch (v2.type()) \
            { \
            case Char: \
                return Variant(v1 op v2.toChar()); \
            case Short: \
                return Variant(v1 op v2.toShort()); \
            case Int: \
                return Variant(v1 op v2.toInt()); \
            case Long: \
                return Variant(v1 op v2.toLong()); \
            case LLong: \
                return Variant(v1 op v2.toLLong()); \
            case Float: \
                return Variant(v1 op v2.toFloat()); \
            case Double: \
                return Variant(v1 op v2.toDouble()); \
            case String: \
                break; \
            case UChar: \
            case UShort: \
            case UInt: \
            case ULong: \
            case ULLong: \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_2_UNSIGNED(op, v1, v2) \
        do \
        { \
            switch (v2.type()) \
            { \
            case UChar: \
                return Variant(v1 op v2.toUChar()); \
            case UShort: \
                return Variant(v1 op v2.toUShort()); \
            case UInt: \
                return Variant(v1 op v2.toUInt()); \
            case ULong: \
                return Variant(v1 op v2.toULong()); \
            case ULLong: \
                return Variant(v1 op v2.toULLong()); \
            case Float: \
                return Variant(v1 op v2.toFloat()); \
            case Double: \
                return Variant(v1 op v2.toDouble()); \
            case String: \
                break; \
            case Char: \
            case Short: \
            case Int: \
            case Long: \
            case LLong: \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_2_NODECIMAL(op, v1, v2) \
        do \
        { \
            switch (v2.type()) \
            { \
            case Char: \
                return Variant(v1 op v2.toChar()); \
            case UChar: \
                return Variant(v1 op v2.toUChar()); \
            case Short: \
                return Variant(v1 op v2.toShort()); \
            case UShort: \
                return Variant(v1 op v2.toUShort()); \
            case Int: \
                return Variant(v1 op v2.toInt()); \
            case UInt: \
                return Variant(v1 op v2.toUInt()); \
            case Long: \
                return Variant(v1 op v2.toLong()); \
            case ULong: \
                return Variant(v1 op v2.toULong()); \
            case LLong: \
                return Variant(v1 op v2.toLLong()); \
            case ULLong: \
                return Variant(v1 op v2.toULLong()); \
            case String: \
                break; \
            case Float: \
            case Double: \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_2_NODECIMAL_SIGNED(op, v1, v2) \
        do \
        { \
            switch (v2.type()) \
            { \
            case Char: \
                return Variant(v1 op v2.toChar()); \
            case Short: \
                return Variant(v1 op v2.toShort()); \
            case Int: \
                return Variant(v1 op v2.toInt()); \
            case UInt: \
            case Long: \
                return Variant(v1 op v2.toLong()); \
            case LLong: \
                return Variant(v1 op v2.toLLong()); \
            case String: \
                break; \
            case UChar: \
            case UShort: \
            case ULong: \
            case ULLong: \
            case Float: \
            case Double: \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_2_NODECIMAL_UNSIGNED(op, v1, v2) \
        do \
        { \
            switch (v2.type()) \
            { \
            case UChar: \
                return Variant(v1 op v2.toUChar()); \
            case UShort: \
                return Variant(v1 op v2.toUShort()); \
            case Int: \
            case UInt: \
                return Variant(v1 op v2.toUInt()); \
            case ULong: \
                return Variant(v1 op v2.toULong()); \
            case ULLong: \
                return Variant(v1 op v2.toULLong()); \
            case String: \
                break; \
            case Char: \
            case Short: \
            case Long: \
            case LLong: \
            case Float: \
            case Double: \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
/* CHECK2_1* */
#define CHECK2_1(op, v) \
        do \
        { \
            switch (_type) \
            { \
            case Char: \
                CHECK2_2(op, toChar(), v); \
                break; \
            case UChar: \
                CHECK2_2(op, toUChar(), v); \
                break; \
            case Short: \
                CHECK2_2(op, toShort(), v); \
                break; \
            case UShort: \
                CHECK2_2(op, toUShort(), v); \
                break; \
            case Int: \
                CHECK2_2(op, toInt(), v); \
                break; \
            case UInt: \
                CHECK2_2(op, toUInt(), v); \
                break; \
            case Long: \
                CHECK2_2(op, toLong(), v); \
                break; \
            case ULong: \
                CHECK2_2(op, toULong(), v); \
                break; \
            case LLong: \
                CHECK2_2(op, toLLong(), v); \
                break; \
            case ULLong: \
                CHECK2_2(op, toULLong(), v); \
                break; \
            case Float: \
                CHECK2_2(op, toFloat(), v); \
                break; \
            case Double: \
                CHECK2_2(op, toDouble(), v); \
                break; \
            case String: \
                break; \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_1_S(op, v) \
        do \
        { \
            switch (_type) \
            { \
            case Char: \
                CHECK2_2_SIGNED(op, toChar(), v); \
                break; \
            case UChar: \
                CHECK2_2_UNSIGNED(op, toUChar(), v); \
                break; \
            case Short: \
                CHECK2_2_SIGNED(op, toShort(), v); \
                break; \
            case UShort: \
                CHECK2_2_UNSIGNED(op, toUShort(), v); \
                break; \
            case Int: \
                CHECK2_2_SIGNED(op, toInt(), v); \
                break; \
            case UInt: \
                CHECK2_2_UNSIGNED(op, toUInt(), v); \
                break; \
            case Long: \
                CHECK2_2_SIGNED(op, toLong(), v); \
                break; \
            case ULong: \
                CHECK2_2_UNSIGNED(op, toULong(), v); \
                break; \
            case LLong: \
                CHECK2_2_SIGNED(op, toLLong(), v); \
                break; \
            case ULLong: \
                CHECK2_2_UNSIGNED(op, toULLong(), v); \
                break; \
            case Float: \
                CHECK2_2(op, toFloat(), v); \
                break; \
            case Double: \
                CHECK2_2(op, toDouble(), v); \
                break; \
            case String: \
                break; \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_1_NODECIMAL(op, v) \
        do \
        { \
            switch (_type) \
            { \
            case Char: \
                CHECK2_2_NODECIMAL(op, toChar(), v); \
                break; \
            case UChar: \
                CHECK2_2_NODECIMAL(op, toUChar(), v); \
                break; \
            case Short: \
                CHECK2_2_NODECIMAL(op, toShort(), v); \
                break; \
            case UShort: \
                CHECK2_2_NODECIMAL(op, toUShort(), v); \
                break; \
            case Int: \
                CHECK2_2_NODECIMAL(op, toInt(), v); \
                break; \
            case UInt: \
                CHECK2_2_NODECIMAL(op, toUInt(), v); \
                break; \
            case Long: \
                CHECK2_2_NODECIMAL(op, toLong(), v); \
                break; \
            case ULong: \
                CHECK2_2_NODECIMAL(op, toULong(), v); \
                break; \
            case LLong: \
                CHECK2_2_NODECIMAL(op, toLLong(), v); \
                break; \
            case ULLong: \
                CHECK2_2_NODECIMAL(op, toULLong(), v); \
                break; \
            case String: \
                break; \
            case Float: \
            case Double: \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
#define CHECK2_1_NODECIMAL_S(op, v) \
        do \
        { \
            switch (_type) \
            { \
            case Char: \
                CHECK2_2_NODECIMAL_SIGNED(op, toChar(), v); \
                break; \
            case UChar: \
                CHECK2_2_NODECIMAL_UNSIGNED(op, toUChar(), v); \
                break; \
            case Short: \
                CHECK2_2_NODECIMAL_SIGNED(op, toShort(), v); \
                break; \
            case UShort: \
                CHECK2_2_NODECIMAL_UNSIGNED(op, toUShort(), v); \
                break; \
            case Int: \
                CHECK2_2_NODECIMAL_SIGNED(op, toInt(), v); \
                break; \
            case UInt: \
                CHECK2_2_NODECIMAL_UNSIGNED(op, toUInt(), v); \
                break; \
            case Long: \
                CHECK2_2_NODECIMAL_SIGNED(op, toLong(), v); \
                break; \
            case ULong: \
                CHECK2_2_NODECIMAL_UNSIGNED(op, toULong(), v); \
                break; \
            case LLong: \
                CHECK2_2_NODECIMAL_SIGNED(op, toLLong(), v); \
                break; \
            case ULLong: \
                CHECK2_2_NODECIMAL_UNSIGNED(op, toULLong(), v); \
                break; \
            case String: \
                break; \
            case Float: \
            case Double: \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)
/* CHECK2* */
#define CHECK2(op, v) CHECK2_1(op, v)
#define CHECK2_S(op, v) CHECK2_1_S(op, v)
#define CHECK2_NODECIMAL(op, v) CHECK2_1_NODECIMAL(op, v)
#define CHECK2_NODECIMAL_S(op, v) CHECK2_1_NODECIMAL_S(op, v)

/* CHECK1 */
#define CHECK1(op) \
        do \
        { \
            switch (_type) \
            { \
            case Char: \
                return Variant(op(toChar())); \
            case UChar: \
                return Variant(op(toUChar())); \
            case Short: \
                return Variant(op(toShort())); \
            case UShort: \
                return Variant(op(toUShort())); \
            case Int: \
                return Variant(op(toInt())); \
            case UInt: \
                return Variant(op(toUInt())); \
            case Long: \
                return Variant(op(toLong())); \
            case ULong: \
                return Variant(op(toULong())); \
            case LLong: \
                return Variant(op(toLLong())); \
            case ULLong: \
                return Variant(op(toULLong())); \
            case Float: \
                return Variant(op(toFloat())); \
            case Double: \
                return Variant(op(toDouble())); \
            case String: \
                break; \
            case Undefined: \
            default: \
                return Variant(); \
            } \
        } while (0)

        const Variant Variant::operator>=(const Variant& v)const
        {
            if (!grantCheck(v, GreaterEqual)) return false;
            CHECK2_S(>=, v);
            return Variant();
        }

        const Variant Variant::operator<=(const Variant& v)const
        {
            if (!grantCheck(v, LessEqual)) return false;
            CHECK2_S(<=, v);
            return Variant();
        }

        const Variant Variant::operator>(const Variant& v)const
        {
            if (!grantCheck(v, Greater)) return false;
            CHECK2_S(>, v);
            return Variant();
        }

        const Variant Variant::operator<(const Variant& v)const
        {
            if (!grantCheck(v, Less)) return false;
            CHECK2_S(<, v);
            return Variant();
        }

        const Variant Variant::operator&&(const Variant& v)const
        {
            if (!grantCheck(v, LogicAnd)) return false;
            CHECK2(&&, v);
            return Variant();
        }

        const Variant Variant::operator||(const Variant& v)const
        {
            if (!grantCheck(v, LogicOr)) return false;
            CHECK2(||, v);
            return Variant();
        }

        const Variant Variant::operator&(const Variant& v)const
        {
            if (!grantCheck(v, BitAnd)) return false;
            CHECK2_NODECIMAL_S(&, v);
            return Variant();
        }

        const Variant Variant::operator|(const Variant& v)const
        {
            if (!grantCheck(v, BitOr)) return false;
            CHECK2_NODECIMAL(|, v);
            return Variant();
        }

        const Variant Variant::operator^(const Variant& v)const
        {
            if (!grantCheck(v, BitXor)) return false;
            CHECK2_NODECIMAL(^, v);
            return Variant();
        }

        const Variant Variant::operator+(const Variant& v)const
        {
            if (!grantCheck(v, Add)) return false;
            CHECK2(+, v);
            return Variant();
        }

        const Variant Variant::operator-(const Variant& v)const
        {
            if (!grantCheck(v, Sub)) return false;
            CHECK2(-, v);
            return Variant();
        }

        const Variant Variant::operator*(const Variant& v)const
        {
            if (!grantCheck(v, Mul)) return false;
            CHECK2(*, v);
            return Variant();
        }

        const Variant Variant::operator/(const Variant& v)const
        {
            if (!grantCheck(v, Div)) return false;
            CHECK2(/, v);
            return Variant();
        }

        const Variant Variant::operator%(const Variant& v)const
        {
            if (!grantCheck(v, Mod)) return false;
            CHECK2_NODECIMAL(%, v);
            return Variant();
        }

        const Variant Variant::operator!()const
        {
            if (!grantCheck(Not)) return false;
            CHECK1(!);
            return Variant();
        }

        const Variant Variant::operator+()const
        {
            if (!grantCheck(Positive)) return false;
            CHECK1(+);
            return true;
        }

        const Variant Variant::operator-()const
        {
            if (!grantCheck(Negative)) return false;
            CHECK1(-);
            return true;
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
