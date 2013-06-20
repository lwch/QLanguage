/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   15:11
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Type.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Type
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_TYPE_H_
#define _QLANGUAGE_SYNTAX_TREE_TYPE_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Type : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            Unsigned = 1,
            Char     = 2,
            Short    = 4,
            Int      = 8,
            Long     = 16,
            Float    = 32,
            Double   = 64,
            String   = 128,
            Auto     = 256,
            Custom   = 512,
            Void     = 1024
        };
    public:
        SyntaxTree_Type(const string& name, ushort type);
        virtual ~SyntaxTree_Type();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const   { return "SyntaxTree_Type";      }
        inline const bool isUnsigned()const { return (_type & Unsigned) != 0; }
        inline const bool isChar()const     { return (_type & Char)     != 0; }
        inline const bool isShort()const    { return (_type & Short)    != 0; }
        inline const bool isInt()const      { return (_type & Int)      != 0; }
        inline const bool isLong()const     { return (_type & Long)     != 0; }
        inline const bool isFloat()const    { return (_type & Float)    != 0; }
        inline const bool isDouble()const   { return (_type & Double)   != 0; }
        inline const bool isString()const   { return (_type & String)   != 0; }
        inline const bool isAuto()const     { return (_type & Auto)     != 0; }
        inline const bool isCustom()const   { return (_type & Custom)   != 0; }
        inline const bool isVoid()const     { return (_type & Void)     != 0; }

        const Type trueType()const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Type*, &x);
#endif
            return name == dynamic_cast<const SyntaxTree_Type*>(&x)->name;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Type*, &x);
#endif
            return name != dynamic_cast<const SyntaxTree_Type*>(&x)->name;
        }
    protected:
        static inline uint inc()
        {
            static uint i = 0;
            return i++;
        }
    protected:
        string name;
        uint   idx;
        ushort _type;
    };
}

#endif
