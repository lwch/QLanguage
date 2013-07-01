/********************************************************************
	created:	2013/07/01
	created:	1:7:2013   22:13
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Template.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Template
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Template.h"

namespace QLanguage
{
    SyntaxTree_Template::SyntaxTree_Template(SyntaxTree_TemplateList* pTemplateList)
        : parent(sizeof(SyntaxTree_Template))
        , pTemplateList(pTemplateList)
    {
    }

    SyntaxTree_Template::SyntaxTree_Template()
        : parent(sizeof(SyntaxTree_Template))
        , pTemplateList(NULL)
    {
    }

    SyntaxTree_Template::~SyntaxTree_Template()
    {
    }

    void SyntaxTree_Template::print(ostream& stream, uint indent)const
    {
        stream << "template <";
        if (pTemplateList) pTemplateList->print(stream, indent);
        stream << '>';
    }

    // template_desc -> "template" "<" template_list ">"
    bool Parser::reduceTemplateWithParamters()
    {
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Template* pTemplate = allocator<SyntaxTree_Template>::allocate();
        construct(pTemplate, dynamic_cast<SyntaxTree_TemplateList*>(syntaxTreeStack.top()));

        context.data.insert(pTemplate);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pTemplate);

        return true;
    }

    // template_desc -> "template" "<" ">"
    bool Parser::reduceTemplateNoParamters()
    {
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Template* pTemplate = allocator<SyntaxTree_Template>::allocate();
        construct(pTemplate);

        context.data.insert(pTemplate);

        syntaxTreeStack.push(pTemplate);

        return true;
    }
}
