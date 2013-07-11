/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   15:13
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Type.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Type
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"

#include "SyntaxTree_Type.h"

namespace QLanguage
{
    SyntaxTree_Type::SyntaxTree_Type(const string& name, ushort type) : parent(sizeof(SyntaxTree_Type)), name(name), idx(inc()), _type(type)
    {
    }

    SyntaxTree_Type::~SyntaxTree_Type()
    {
    }

    void SyntaxTree_Type::print(ostream& stream, uint indent)const
    {
        stream << name;
        for (size_t i = 0, m = childs.size(); i < m; ++i)
        {
            stream << ' ';
            childs[i]->print(stream, indent);
            if (i < m - 1) stream << ',';
        }
    }

    const SyntaxTree_Type::Type SyntaxTree_Type::trueType()const
    {
        switch (_type & ~Unsigned)
        {
        case Char:   return Char;
        case Short:  return Short;
        case Int:    return Int;
        case Long:   return Long;
        case Float:  return Float;
        case Double: return Double;
        case String: return String;
        case Auto:   return Auto;
        case Void:   return Void;
        default:     return Custom;
        }
    }

    const HASH_KEY_TYPE SyntaxTree_Type::hash()const
    {
        HASH_KEY_TYPE result = ::hash<string>()(name);
        if (isUnsigned()) result += ::hash<string>()("unsigned");
        return result;
    }

    // type_desc -> "unsigned" "char"
    // type_desc -> "unsigned" "short"
    // type_desc -> "unsigned" "int"
    // type_desc -> "unsigned" "long"
    bool Parser::reduceType2Size(ushort i)
    {
        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        switch (i)
        {
        case TYPE_DESC_UNSIGNED_CHAR:  // type_desc -> "unsigned" "char"
            construct(pType, shifts.top(), SyntaxTree_Type::Unsigned | SyntaxTree_Type::Char);
            break;
        case TYPE_DESC_UNSIGNED_SHORT: // type_desc -> "unsigned" "short"
            construct(pType, shifts.top(), SyntaxTree_Type::Unsigned | SyntaxTree_Type::Short);
            break;
        case TYPE_DESC_UNSIGNED_INT:   // type_desc -> "unsigned" "int"
            construct(pType, shifts.top(), SyntaxTree_Type::Unsigned | SyntaxTree_Type::Int);
            break;
        case TYPE_DESC_UNSIGNED_LONG:  // type_desc -> "unsigned" "long"
            construct(pType, shifts.top(), SyntaxTree_Type::Unsigned | SyntaxTree_Type::Long);
            break;
        }

        context.data.insert(pType);

        syntaxTreeStack.push(pType);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // type_desc -> "char"
    // type_desc -> "short"
    // type_desc -> "int"
    // type_desc -> "long"
    // type_desc -> "float"
    // type_desc -> "double"
    // type_desc -> "string"
    // type_desc -> "auto"
    // type_desc -> "{Letter}"
    bool Parser::reduceType1Size(ushort i)
    {
        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        switch (i)
        {
        case TYPE_DESC_CHAR:   // type_desc -> "char"
            construct(pType, shifts.top(), SyntaxTree_Type::Char);
            break;
        case TYPE_DESC_SHORT:  // type_desc -> "short"
            construct(pType, shifts.top(), SyntaxTree_Type::Short);
            break;
        case TYPE_DESC_INT:    // type_desc -> "int"
            construct(pType, shifts.top(), SyntaxTree_Type::Int);
            break;
        case TYPE_DESC_LONG:   // type_desc -> "long"
            construct(pType, shifts.top(), SyntaxTree_Type::Long);
            break;
        case TYPE_DESC_FLOAT:  // type_desc -> "float"
            construct(pType, shifts.top(), SyntaxTree_Type::Float);
            break;
        case TYPE_DESC_DOUBLE: // type_desc -> "double"
            construct(pType, shifts.top(), SyntaxTree_Type::Double);
            break;
        case TYPE_DESC_STRING: // type_desc -> "string"
            construct(pType, shifts.top(), SyntaxTree_Type::String);
            break;
        case TYPE_DESC_AUTO:   // type_desc -> "auto"
            construct(pType, shifts.top(), SyntaxTree_Type::Auto);
            break;
        case TYPE_DESC_LETTER: // type_desc -> "{Letter}"
            construct(pType, shifts.top(), SyntaxTree_Type::Custom);
            break;
        }

        context.data.insert(pType);

        syntaxTreeStack.push(pType);

        shifts.pop();
        return true;
    }
}
