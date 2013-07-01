/********************************************************************
	created:	2013/07/01
	created:	1:7:2013   21:35
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_TemplateItem.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_TemplateItem
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_TemplateItem.h"

namespace QLanguage
{
    SyntaxTree_TemplateItem::SyntaxTree_TemplateItem(const string& name)
        : parent(sizeof(SyntaxTree_TemplateItem))
        , pType(NULL)
        , _type(_TypeName)
        , name(name)
    {
    }

    SyntaxTree_TemplateItem::SyntaxTree_TemplateItem(SyntaxTree_Type* pType, const string& name)
        : parent(sizeof(SyntaxTree_TemplateItem))
        , pType(pType)
        , _type(_Type)
        , name(name)
    {
    }

    SyntaxTree_TemplateItem::~SyntaxTree_TemplateItem()
    {
    }

    void SyntaxTree_TemplateItem::print(ostream& stream, uint indent)const
    {
        switch (_type)
        {
        case _TypeName:
            stream << "typename " << name;
            break;
        case _Type:
            pType->print(stream, indent);
            stream << ' ' << name;
            break;
        }
    }

    // template_item -> "typename" "{Letter}"
    bool Parser::reduceTemplateItemWithTypeName()
    {
        SyntaxTree_TemplateItem* pTemplateItem = allocator<SyntaxTree_TemplateItem>::allocate();
        construct(pTemplateItem, shifts.top());

        context.data.insert(pTemplateItem);

        syntaxTreeStack.push(pTemplateItem);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // template_item -> type_desc "{Letter}"
    bool Parser::reduceTemplateItemWithType()
    {
        SyntaxTree_TemplateItem* pTemplateItem = allocator<SyntaxTree_TemplateItem>::allocate();
        construct(pTemplateItem, dynamic_cast<SyntaxTree_Type*>(syntaxTreeStack.top()), shifts.top());

        context.data.insert(pTemplateItem);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pTemplateItem);

        shifts.pop();

        return true;
    }
}
