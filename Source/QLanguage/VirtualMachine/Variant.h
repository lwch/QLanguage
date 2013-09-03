/********************************************************************
	created:	2013/07/13
	created:	13:7:2013   15:05
	filename: 	\QLanguage\VirtualMachine\Variant.h
	file path:	\QLanguage\VirtualMachine
	file base:	Variant
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_VARIANT_H_
#define _QLANGUAGE_VARIANT_H_

#include "../../QCore/Library/error.h"
#include "../../QCore/Library/string.h"
#include "../../QCore/Library/typedef.h"

namespace QLanguage
{
    using namespace Library;

    namespace VM
    {
        class Variant
        {
            enum
            {
                GreaterEqual,
                LessEqual,
                Equal,
                Greater,
                Less,
                LogicAnd,
                LogicOr,
                BitAnd,
                BitOr,
                BitXor,
                Add,
                Sub,
                Mul,
                Div,
                Mod
            };
        public:
            enum Type
            {
                Char,
                UChar,
                Short,
                UShort,
                Int,
                UInt,
                Long,
                ULong,
                LLong,
                ULLong,
                Float,
                Double,
                String,
                Undefined
            };
        public:
            Variant();
            Variant(char   x);
            Variant(uchar  x);
            Variant(short  x);
            Variant(ushort x);
            Variant(int    x);
            Variant(uint   x);
            Variant(long   x);
            Variant(ulong  x);
            Variant(llong  x);
            Variant(ullong x);
            Variant(float  x);
            Variant(double x);
            Variant(const char* x);
            Variant(const char* x, size_t start, size_t count = -1);
            Variant(const string& x);
            Variant(const string& x, size_t start, size_t count = -1);
            ~Variant();

            inline const Type type()const { return _type; }

            inline const bool toBool()const
            {
                return number2Type<bool>();
            }

            inline const char toChar()const
            {
                return number2Type<char>();
            }

            inline const uchar toUChar()const
            {
                return number2Type<uchar>();
            }

            inline const short toShort()const
            {
                return number2Type<short>();
            }

            inline const ushort toUShort()const
            {
                return number2Type<ushort>();
            }

            inline const int toInt()const
            {
                return number2Type<int>();
            }

            inline const uint toUInt()const
            {
                return number2Type<uint>();
            }

            inline const long toLong()const
            {
                return number2Type<long>();
            }

            inline const ulong toULong()const
            {
                return number2Type<ulong>();
            }

            inline const llong toLLong()const
            {
                return number2Type<llong>();
            }

            inline const ullong toULLong()const
            {
                return number2Type<ullong>();
            }

            inline const float toFloat()const
            {
                return number2Type<float>();
            }

            inline const double toDouble()const
            {
                return number2Type<double>();
            }

            const char* toCharPointer()const;
            const string toString()const;

            const bool operator==(const Variant& v)const;

            const Variant operator>=(const Variant& v)const;
            const Variant operator<=(const Variant& v)const;
            const Variant operator>(const Variant& v)const;
            const Variant operator<(const Variant& v)const;
            const Variant operator&&(const Variant& v)const;
            const Variant operator||(const Variant& v)const;
            const Variant operator&(const Variant& v)const;
            const Variant operator|(const Variant& v)const;
            const Variant operator^(const Variant& v)const;
            const Variant operator+(const Variant& v)const;
            const Variant operator-(const Variant& v)const;
            const Variant operator*(const Variant& v)const;
            const Variant operator/(const Variant& v)const;
            const Variant operator%(const Variant& v)const;
        protected:
            const bool compareString(const char* data, size_t len)const;
        protected:
            template <typename T>
            const T number2Type()const
            {
                switch (_type)
                {
                case Char:
                    return static_cast<T>(charValue);
                case UChar:
                    return static_cast<T>(ucharValue);
                case Short:
                    return static_cast<T>(shortValue);
                case UShort:
                    return static_cast<T>(ushortValue);
                case Int:
                    return static_cast<T>(intValue);
                case UInt:
                    return static_cast<T>(uintValue);
                case Long:
                    return static_cast<T>(longValue);
                case ULong:
                    return static_cast<T>(ulongValue);
                case LLong:
                    return static_cast<T>(llongValue);
                case ULLong:
                    return static_cast<T>(ullongValue);
                case Float:
                    return static_cast<T>(floatValue);
                case Double:
                    return static_cast<T>(doubleValue);
                case String:
                    throw error<const char*>("Can't cast from string to number", __FILE__, __LINE__);
                case Undefined:
                    throw error<const char*>("Variant has no value", __FILE__, __LINE__);
                }
                return T();
            }

#ifdef MSVC
            template <>
#endif
            const bool number2Type()const
            {
                switch (_type)
                {
                case Char:
                    return charValue != 0;
                case UChar:
                    return ucharValue != 0;
                case Short:
                    return shortValue != 0;
                case UShort:
                    return ushortValue != 0;
                case Int:
                    return intValue != 0;
                case UInt:
                    return uintValue != 0;
                case Long:
                    return longValue != 0;
                case ULong:
                    return ulongValue != 0;
                case LLong:
                    return llongValue != 0;
                case ULLong:
                    return ullongValue != 0;
                case Float:
                    return floatValue != 0;
                case Double:
                    return doubleValue != 0;
                case String:
                    throw error<const char*>("Can't cast from string to bool", __FILE__, __LINE__);
                case Undefined:
                    throw error<const char*>("Variant has no value", __FILE__, __LINE__);
                }
                return bool();
            }

            void initString(const char* x, size_t size, size_t start, size_t count = -1);
        protected:
            union
            {
                char   charValue;
                uchar  ucharValue;
                short  shortValue;
                ushort ushortValue;
                int    intValue;
                uint   uintValue;
                long   longValue;
                ulong  ulongValue;
                llong  llongValue;
                ullong ullongValue;
                float  floatValue;
                double doubleValue;

                struct 
                {
                    char*  value;
                    size_t length;
                }stringValue;
            };

            Type _type;
        };
    }
}

#endif
