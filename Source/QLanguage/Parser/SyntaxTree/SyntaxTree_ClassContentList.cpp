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
#include "SyntaxTree_ClassContent.h"
#include "SyntaxTree_ClassContentList.h"

namespace QLanguage
{
    SyntaxTree_ClassContentList::SyntaxTree_ClassContentList()
        : parent(sizeof(SyntaxTree_ClassContentList))
    {
    }

    SyntaxTree_ClassContentList::~SyntaxTree_ClassContentList()
    {
    }

    void SyntaxTree_ClassContentList::print(ostream& stream, uint indent)const
    {
        for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(); i != m; ++i)
        {
            this->printIndent(stream, indent + parent::indent);
            (*i)->print(stream, indent + parent::indent);
            stream << endl;
        }
    }

    // class_content_list -> class_content_list class_content
    bool Parser::reduceClassContentList2Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ClassContentList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassContent*, syntaxTreeStack.top());
#endif
        SyntaxTree_ClassContentList* pContentList = dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack[1]);
        
        pContentList->pushChild(syntaxTreeStack.top());
        
        syntaxTreeStack.pop();
        
        return true;
    }

    // class_content_list -> class_content
    bool Parser::reduceClassContentList1Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ClassContent*, syntaxTreeStack.top());
#endif
        SyntaxTree_ClassContentList* pContentList = allocator<SyntaxTree_ClassContentList>::allocate();
        construct(pContentList);
        
        context.data.insert(pContentList);
        
        pContentList->pushChild(syntaxTreeStack.top());
        
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pContentList);
        
        return true;
    }
}
