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
    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(pTemplate)
        , name(name)
        , pInherit(pInherit)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(NULL)
        , name(name)
        , pInherit(pInherit)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(pTemplate)
        , name(name)
        , pInherit(NULL)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassContentList* pContentList)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(NULL)
        , name(name)
        , pInherit(NULL)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(pTemplate)
        , name(name)
        , pInherit(pInherit)
        , pContentList(NULL)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(NULL)
        , name(name)
        , pInherit(pInherit)
        , pContentList(NULL)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(pTemplate)
        , name(name)
        , pInherit(NULL)
        , pContentList(NULL)
    {
    }

    SyntaxTree_Class::SyntaxTree_Class(const SyntaxTree_ClassName& name)
        : parent(sizeof(SyntaxTree_Class))
        , pTemplate(NULL)
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
        if (pTemplate)
        {
            pTemplate->print(stream, indent);
            stream << ' ';
        }
        name.print(stream, indent);
        if (pInherit) pInherit->print(stream, indent);
        stream << " {" << endl;
        if (pContentList) pContentList->print(stream, indent);
        stream << '}' << endl;
    }

    // class_desc -> template_desc class_desc1 class_desc2 "{" class_content_list "}"
    bool Parser::reduceClass1()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ClassInherit*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassContentList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[3]),
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[2]),
                  dynamic_cast<SyntaxTree_ClassInherit*>(syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // class_desc -> class_desc1 class_desc2 "{" class_content_list "}"
    bool Parser::reduceClass2()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ClassInherit*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassContentList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[2]),
                  dynamic_cast<SyntaxTree_ClassInherit*>(syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // class_desc -> template_desc class_desc1 "{" class_content "}"
    bool Parser::reduceClass3()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassContentList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack.top()));

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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassContentList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_ClassContentList*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // class_desc -> template_desc class_desc1 class_desc2 "{" "}"
    bool Parser::reduceClass5()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassInherit*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_ClassInherit*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassInherit*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack[1]),
                  dynamic_cast<SyntaxTree_ClassInherit*>(syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // class_desc -> template_desc class_desc1 "{" "}"
    bool Parser::reduceClass7()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack.top()));

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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ClassName*, syntaxTreeStack.top());
#endif
        SyntaxTree_Class* pClass = allocator<SyntaxTree_Class>::allocate();
        construct(pClass,
                  dynamic_cast<const SyntaxTree_ClassName&>(*syntaxTreeStack.top()));

        context.data.insert(pClass);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClass);

        shifts.pop();
        shifts.pop();

        return true;
    }
}
