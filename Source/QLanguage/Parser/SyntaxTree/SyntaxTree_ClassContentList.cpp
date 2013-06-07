/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   17:03
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ClassContentList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ClassContentList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ClassContentList.h"

namespace QLanguage
{
    SyntaxTree_ClassContentList::SyntaxTree_ClassContentList() : parent(sizeof(SyntaxTree_ClassContentList))
    {
    }

    SyntaxTree_ClassContentList::~SyntaxTree_ClassContentList()
    {
    }

    void SyntaxTree_ClassContentList::print(ostream& stream, uint indent)const
    {
    }

    // class_content -> class_content function_desc
    // class_content -> class_content attribute declare_desc ";"
    // class_content -> class_content declare_desc ";"
    bool Parser::reduceClassContent2Size(ushort i)
    {
        SyntaxTree_ClassContentList* pClassContentList = dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack[1]);

        context.data.insert(pClassContentList);

        if (i == CLASS_CONTENT_CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC) pClassContentList->pushChild(syntaxTreeStack[1]);
        pClassContentList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        if (i == CLASS_CONTENT_CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC) syntaxTreeStack.pop();

        if (i == CLASS_CONTENT_CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC ||
            i == CLASS_CONTENT_CLASS_CONTENT_DECLARE_DESC)
            shifts.pop();
        return true;
    }

    // class_content -> function_desc
    // class_content -> attribute declare_desc ";"
    // class_content -> declare_desc ";"
    bool Parser::reduceClassContent1Size(ushort i)
    {
        SyntaxTree_ClassContentList* pClassContentList = allocator<SyntaxTree_ClassContentList>::allocate();
        construct(pClassContentList);

        context.data.insert(pClassContentList);

        if (i == CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC) pClassContentList->pushChild(syntaxTreeStack[1]);
        pClassContentList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        if (i == CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC) syntaxTreeStack.pop();

        if (i == CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC ||
            i == CLASS_CONTENT_ATTRIBUTE_DECLARE_DESC)
            shifts.pop();
        return true;
    }
}
