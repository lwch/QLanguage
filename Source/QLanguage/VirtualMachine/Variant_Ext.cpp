/********************************************************************
	created:	2013/09/03
	created:	3:9:2013   23:28
	filename: 	\QLanguage\VirtualMachine\Variant_Ext.cpp
	file path:	\QLanguage\VirtualMachine
	file base:	Variant_Ext
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "Variant.h"

namespace QLanguage
{
    namespace VM
    {
        // 权限相关
        // TODO: 检查权限是否都正确
#define vGrantTable_Normal1 \
        { \
            Variant::Char, \
            Variant::UChar, \
            Variant::Short, \
            Variant::UShort, \
            Variant::Int, \
            Variant::UInt, \
            Variant::Long, \
            Variant::ULong, \
            Variant::LLong, \
            Variant::ULLong, \
            Variant::Float, \
            Variant::Double \
        }

#define vGrantTable_Normal2 \
        { \
            Variant::Char, \
            Variant::UChar, \
            Variant::Short, \
            Variant::UShort, \
            Variant::Int, \
            Variant::UInt, \
            Variant::Long, \
            Variant::ULong, \
            Variant::LLong, \
            Variant::ULLong, \
            Variant::Float, \
            Variant::Double \
        }

        // 一元操作符权限
        // vGrantTable_Not_*
        uchar vGrantTable_Not[] = vGrantTable_Normal1;

        // vGrantTable_Positive_*
        uchar vGrantTable_Positive[] = vGrantTable_Normal1;

        // vGrantTable_Negative_*
        uchar vGrantTable_Negative[] = vGrantTable_Normal1;

        // 二元操作符权限
        // vGrantTable_GreaterEqual_*
        uchar vGrantTable_GreaterEqual_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_GreaterEqual_String[] = {};
        uchar vGrantTable_GreaterEqual_Undefined[] = {};

        // vGrantTable_LessEqual_*
        uchar vGrantTable_LessEqual_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_LessEqual_String[] = {};
        uchar vGrantTable_LessEqual_Undefined[] = {};

        // vGrantTable_Equal_*
        uchar vGrantTable_Equal_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Equal_String[] = {Variant::String};
        uchar vGrantTable_Equal_Undefined[] = {};

        // vGrantTable_Greater_*
        uchar vGrantTable_Greater_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Greater_String[] = {};
        uchar vGrantTable_Greater_Undefined[] = {};

        // vGrantTable_Less_*
        uchar vGrantTable_Less_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Less_String[] = {};
        uchar vGrantTable_Less_Undefined[] = {};

        // vGrantTable_LogicAnd_*
        uchar vGrantTable_LogicAnd_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicAnd_String[] = {};
        uchar vGrantTable_LogicAnd_Undefined[] = {};

        // vGrantTable_LogicOr_*
        uchar vGrantTable_LogicOr_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_LogicOr_String[] = {};
        uchar vGrantTable_LogicOr_Undefined[] = {};

        // vGrantTable_BitAnd_*
        uchar vGrantTable_BitAnd_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_BitAnd_String[] = {};
        uchar vGrantTable_BitAnd_Undefined[] = {};

        // vGrantTable_BitOr_*
        uchar vGrantTable_BitOr_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_BitOr_String[] = {};
        uchar vGrantTable_BitOr_Undefined[] = {};

        // vGrantTable_BitXor_*
        uchar vGrantTable_BitXor_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_BitXor_String[] = {};
        uchar vGrantTable_BitXor_Undefined[] = {};

        // vGrantTable_Add_*
        uchar vGrantTable_Add_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Add_String[] = {};
        uchar vGrantTable_Add_Undefined[] = {};

        // vGrantTable_Sub_*
        uchar vGrantTable_Sub_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Sub_String[] = {};
        uchar vGrantTable_Sub_Undefined[] = {};

        // vGrantTable_Mul_*
        uchar vGrantTable_Mul_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Mul_String[] = {};
        uchar vGrantTable_Mul_Undefined[] = {};

        // vGrantTable_Div_*
        uchar vGrantTable_Div_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Div_String[] = {};
        uchar vGrantTable_Div_Undefined[] = {};

        // vGrantTable_Mod_*
        uchar vGrantTable_Mod_Char[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_UChar[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_Short[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_UShort[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_Int[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_UInt[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_Long[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_ULong[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_LLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_ULLong[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_Float[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_Double[] = vGrantTable_Normal2;
        uchar vGrantTable_Mod_String[] = {};
        uchar vGrantTable_Mod_Undefined[] = {};

#define GRANTTABLE_LOOKUP(table, type, out) \
        do \
        { \
            out = -1; \
            for (int i = 0; i < sizeof(table); ++i) \
            { \
                if (table[i] == type) \
                { \
                    out = i; \
                    break; \
                } \
            } \
        } while (0)

#define GRANTTABLE_NAME1(op) vGrantTable_##op
#define GRANTTABLE_NAME2(op, type) vGrantTable_##op##_##type
#define GRANTTABLE_LOOKUP2(op, dstType, out) \
        do \
        { \
            switch (_type) \
            { \
            case Char: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, Char), dstType, out); \
                break; \
            case UChar: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, UChar), dstType, out); \
                break; \
            case Short: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, Short), dstType, out); \
                break; \
            case UShort: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, UShort), dstType, out); \
                break; \
            case Int: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, Int), dstType, out); \
                break; \
            case UInt: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, UInt), dstType, out); \
                break; \
            case Long: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, Long), dstType, out); \
                break; \
            case ULong: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, ULong), dstType, out); \
                break; \
            case LLong: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, LLong), dstType, out); \
                break; \
            case ULLong: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, ULLong), dstType, out); \
                break; \
            case Float: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, Float), dstType, out); \
                break; \
            case Double: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, Double), dstType, out); \
                break; \
            case String: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, String), dstType, out); \
                break; \
            case Undefined: \
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME2(op, Undefined), dstType, out); \
                break; \
            default: \
                out = -1; \
                break; \
            } \
        } while (0)

        const bool Variant::grantCheck(OperatorType type)const
        {
            int idx = -1;
            switch (type)
            {
            case Not:
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME1(Not), _type, idx);
                break;
            case Positive:
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME1(Positive), _type, idx);
                break;
            case Negative:
                GRANTTABLE_LOOKUP(GRANTTABLE_NAME1(Negative), _type, idx);
                break;
            default:
                return false;
            }
            return idx > -1;
        }

        const bool Variant::grantCheck(const Variant& v, OperatorType type)const
        {
            int idx = -1;
            switch (type)
            {
            case GreaterEqual:
                GRANTTABLE_LOOKUP2(GreaterEqual, v.type(), idx);
                break;
            case LessEqual:
                GRANTTABLE_LOOKUP2(LessEqual, v.type(), idx);
                break;
            case Equal:
                GRANTTABLE_LOOKUP2(Equal, v.type(), idx);
                break;
            case Greater:
                GRANTTABLE_LOOKUP2(Greater, v.type(), idx);
                break;
            case Less:
                GRANTTABLE_LOOKUP2(Less, v.type(), idx);
                break;
            case LogicAnd:
                GRANTTABLE_LOOKUP2(LogicAnd, v.type(), idx);
                break;
            case LogicOr:
                GRANTTABLE_LOOKUP2(LogicOr, v.type(), idx);
                break;
            case BitAnd:
                GRANTTABLE_LOOKUP2(BitAnd, v.type(), idx);
                break;
            case BitOr:
                GRANTTABLE_LOOKUP2(BitOr, v.type(), idx);
                break;
            case BitXor:
                GRANTTABLE_LOOKUP2(BitXor, v.type(), idx);
                break;
            case Add:
                GRANTTABLE_LOOKUP2(Add, v.type(), idx);
                break;
            case Sub:
                GRANTTABLE_LOOKUP2(Sub, v.type(), idx);
                break;
            case Mul:
                GRANTTABLE_LOOKUP2(Mul, v.type(), idx);
                break;
            case Div:
                GRANTTABLE_LOOKUP2(Div, v.type(), idx);
                break;
            case Mod:
                GRANTTABLE_LOOKUP2(Mod, v.type(), idx);
                break;
            default:
                return false;
            }
            return idx > -1;
        }
    }
}

