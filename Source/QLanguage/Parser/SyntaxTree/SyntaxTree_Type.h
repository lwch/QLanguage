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

        inline const bool isUnsigned()const { return (type & Unsigned) != 0; }
        inline const bool isChar()const     { return (type & Char)     != 0; }
        inline const bool isShort()const    { return (type & Short)    != 0; }
        inline const bool isInt()const      { return (type & Int)      != 0; }
        inline const bool isLong()const     { return (type & Long)     != 0; }
        inline const bool isFloat()const    { return (type & Float)    != 0; }
        inline const bool isDouble()const   { return (type & Double)   != 0; }
        inline const bool isString()const   { return (type & String)   != 0; }
        inline const bool isAuto()const     { return (type & Auto)     != 0; }
        inline const bool isCustom()const   { return (type & Custom)   != 0; }
        inline const bool isVoid()const     { return (type & Void)     != 0; }

        const Type trueType()const;

        const bool operator==(const SyntaxTree_Type& x)const;
        const bool operator==(const string& x)const;
        const bool operator!=(const SyntaxTree_Type& x)const;
    protected:
        static inline uint inc()
        {
            static uint i = 0;
            return i++;
        }
    protected:
        string name;
        uint   idx;
        ushort type;
    };
}

#endif
