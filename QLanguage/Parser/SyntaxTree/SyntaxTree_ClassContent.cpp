#include "../Parser.h"
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

    SyntaxTree_ClassContent::SyntaxTree_ClassContent(SyntaxTree_Attribute* pAttribute, const SyntaxTree_Type& content)
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
        shifts.pop();
        
        SyntaxTree_ClassContent* pContent = allocator<SyntaxTree_ClassContent>::allocate();
        construct(pContent,
                  dynamic_cast<SyntaxTree_Attribute*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack.top()));
        
        context.data.insert(pContent);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pContent);
        
        return true;
    }
    
    // class_content -> declare_desc ";"
    bool Parser::reduceClassContentDeclareNoAttribute()
    {
        shifts.pop();
        
        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_ClassContent::defaultAttribute);
        
        context.data.insert(pAttribute);
        
        SyntaxTree_ClassContent* pContent = allocator<SyntaxTree_ClassContent>::allocate();
        construct(pContent,
                  pAttribute,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack.top()));
        
        context.data.insert(pContent);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pContent);
        
        return true;
    }
}
