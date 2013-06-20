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
    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , name(name)
        , pInherit(pInherit)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , name(name)
        , pInherit(NULL)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit)
        : parent(sizeof(SyntaxTree_Class))
        , name(name)
        , pInherit(pInherit)
        , pContentList(NULL)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name)
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
        this->printIndent(stream, indent);
        name.print(stream, indent);
        if (pInherit) pInherit->print(stream, indent);
        stream << " {" << endl;
        if (pContentList) pContentList->print(stream, indent);
        stream << '}' << endl;
    }

    // class_desc -> class_desc1 class_desc2 "{" class_content "}"
    bool Parser::reduceClass2()
    {
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass, dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[2]), dynamic_cast<SyntaxTree_ClassInherit*>(syntaxTreeStack[1]), dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // class_desc -> class_desc1 "{" class_content "}"
    bool Parser::reduceClass4()
    {
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass, dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[1]), dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // class_desc -> class_desc1 class_desc2 "{" "}"
    bool Parser::reduceClass6()
    {
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass, dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[1]), dynamic_cast<SyntaxTree_ClassInherit*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // class_desc -> class_desc1 "{" "}"
    bool Parser::reduceClass8()
    {
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass, dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();
        return true;
    }
}
