/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:09
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ClassContent.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ClassContent
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Function.h"
#include "SyntaxTree_DeclareList.h"
#include "SyntaxTree_ClassContent.h"

namespace QLanguage
{
    SyntaxTree_Attribute::Type SyntaxTree_ClassContent::defaultAttribute = SyntaxTree_Attribute::Private;
    
    SyntaxTree_ClassContent::SyntaxTree_ClassContent(const SyntaxTree_Base& content)
        : parent(sizeof(SyntaxTree_ClassContent))
        , pAttribute(NULL)
        , content(content)
        , _type(Function)
    {
    }

    SyntaxTree_ClassContent::SyntaxTree_ClassContent(SyntaxTree_Attribute* pAttribute, const SyntaxTree_DeclareList& content)
        : parent(sizeof(SyntaxTree_ClassContent))
        , pAttribute(pAttribute)
        , content(content)
        , _type(Declare)
    {
    }


    SyntaxTree_ClassContent::~SyntaxTree_ClassContent()
    {
    }

    void SyntaxTree_ClassContent::print(ostream& stream, uint indent) const
    {
        if (_type == Declare)
        {
            pAttribute->print(stream, indent);
            stream << ' ';
        }
        content.print(stream, indent);
        if (_type == Declare) stream << ';';
    }

    // class_content -> function_desc
    bool Parser::reduceClassContentFunction()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Function*, syntaxTreeStack.top());
#endif
        SyntaxTree_ClassContent* pContent = allocator<SyntaxTree_ClassContent>::allocate();
        construct(pContent, *syntaxTreeStack.top());
        
        context.data.insert(pContent);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pContent);
        
        return true;
    }
    
    // class_content -> attribute declare_desc ";"
    bool Parser::reduceClassContentDeclareWithAttribute()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_DeclareList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        
        SyntaxTree_ClassContent* pContent = allocator<SyntaxTree_ClassContent>::allocate();
        construct(pContent,
                  dynamic_cast<SyntaxTree_Attribute*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_DeclareList&>(*syntaxTreeStack.top()));
        
        context.data.insert(pContent);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pContent);
        
        return true;
    }
    
    // class_content -> declare_desc ";"
    bool Parser::reduceClassContentDeclareNoAttribute()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_DeclareList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        
        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_ClassContent::defaultAttribute);
        
        context.data.insert(pAttribute);
        
        SyntaxTree_ClassContent* pContent = allocator<SyntaxTree_ClassContent>::allocate();
        construct(pContent,
                  pAttribute,
                  dynamic_cast<const SyntaxTree_DeclareList&>(*syntaxTreeStack.top()));
        
        context.data.insert(pContent);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pContent);
        
        return true;
    }
}
