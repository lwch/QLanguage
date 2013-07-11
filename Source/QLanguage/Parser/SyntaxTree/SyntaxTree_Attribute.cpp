/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   14:22
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Attribute.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Attribute
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Attribute.h"

namespace QLanguage
{
    SyntaxTree_Attribute::SyntaxTree_Attribute(Type type) : parent(sizeof(SyntaxTree_Attribute)), _type(type)
    {
    }

    SyntaxTree_Attribute::~SyntaxTree_Attribute()
    {
    }

    void SyntaxTree_Attribute::print(ostream& stream, uint indent)const
    {
        switch (_type)
        {
        case Public:
            stream << "public";
            break;
        case Private:
            stream << "private";
            break;
        case Protected:
            stream << "protected";
            break;
        }
    }

    const HASH_KEY_TYPE SyntaxTree_Attribute::hash()const
    {
        return ::hash<string>()(_type == Public ? "public" : _type == Private ? "private" : "protected");
    }

    // attribute -> "public"
    // attribute -> "private"
    // attribute -> "protected"
    bool Parser::reduceAttribute(ushort i)
    {
        SyntaxTree_Attribute* pAttrivute = allocator<SyntaxTree_Attribute>::allocate();
        switch (i)
        {
        case ATTRIBUTE_PUBLIC:
            construct(pAttrivute, SyntaxTree_Attribute::Public);
            break;
        case ATTRIBUTE_PRIVATE:
            construct(pAttrivute, SyntaxTree_Attribute::Private);
            break;
        case ATTRIBUTE_PROTECTED:
            construct(pAttrivute, SyntaxTree_Attribute::Protected);
            break;
        }

        context.data.insert(pAttrivute);

        syntaxTreeStack.push(pAttrivute);

        shifts.pop();

        return true;
    }
}
