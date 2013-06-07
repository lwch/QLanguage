/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   13:12
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_InterfaceContentList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_InterfaceContentList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_InterfaceContentList.h"

namespace QLanguage
{
    SyntaxTree_InterfaceContentList::SyntaxTree_InterfaceContentList() : parent(sizeof(SyntaxTree_InterfaceContentList))
    {
    }

    SyntaxTree_InterfaceContentList::~SyntaxTree_InterfaceContentList()
    {
    }

    void SyntaxTree_InterfaceContentList::print(ostream& stream, uint indent)const
    {
    }

    // interface_content -> interface_content function_declare
    bool Parser::reduceInterfaceContent2Size()
    {
        SyntaxTree_InterfaceContentList* pInterfaceContentList = dynamic_cast<SyntaxTree_InterfaceContentList*>(syntaxTreeStack[1]);

        pInterfaceContentList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        return true;
    }

    // interface_content -> function_declare
    bool Parser::reduceInterfaceContent1Size()
    {
        SyntaxTree_InterfaceContentList* pInterfaceContentList = allocator<SyntaxTree_InterfaceContentList>::allocate();
        construct(pInterfaceContentList);

        context.data.insert(pInterfaceContentList);

        pInterfaceContentList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pInterfaceContentList);
        return true;
    }
}
