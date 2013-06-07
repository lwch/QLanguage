/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   17:36
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Class.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Class
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"

#include "SyntaxTree_ClassName.h"
#include "SyntaxTree_ClassInherit.h"
#include "SyntaxTree_Class.h"

namespace QLanguage
{
    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , name(name)
        , pInherit(pInherit)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_ClassName& name, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , name(name)
        , pInherit(NULL)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit)
        : parent(sizeof(SyntaxTree_Class))
        , name(name)
        , pInherit(pInherit)
        , pContentList(NULL)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_ClassName& name)
        : parent(sizeof(SyntaxTree_Class))
        , name(name)
        , pInherit(NULL)
        , pContentList(NULL)
    {
    }

    SyntaxTree_Class::~SyntaxTree_Class()
    {
    }

    void SyntaxTree_Class::print(ostream& stream, uint indent)const
    {
    }

    // class_desc -> class_desc1 class_desc2 "{" class_content "}"
    bool Parser::reduceClass2()
    {
        // TODO
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass, dynamic_cast<SyntaxTree_ClassName&>(*syntaxTreeStack[2]), dynamic_cast<SyntaxTree_ClassInherit*>(syntaxTreeStack[1]), syntaxTreeStack.top());
        return true;
    }

    // class_desc -> class_desc1 "{" class_content "}"
    bool Parser::reduceClass4()
    {
        // TODO
        return true;
    }

    // class_desc -> class_desc1 class_desc2 "{" "}"
    bool Parser::reduceClass6()
    {
        // TODO
        return true;
    }

    // class_desc -> class_desc1 "{" "}"
    bool Parser::reduceClass8()
    {
        // TODO
        return true;
    }
}
