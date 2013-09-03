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
#define vGrantTable_Normal \
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

        // 二元操作符权限
        // vGrantTable_GreaterEqual_*
        uchar vGrantTable_GreaterEqual_Char[] = vGrantTable_Normal;
        uchar vGrantTable_GreaterEqual_UChar[] = vGrantTable_Normal;
        uchar vGrantTable_GreaterEqual_Short[] = vGrantTable_Normal;
        // TODO: ...
    }
}
