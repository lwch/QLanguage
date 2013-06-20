#ifndef _QLANGUAGE_SYNTAX_TREE_CLASS_CONTENT_H_
#define _QLANGUAGE_SYNTAX_TREE_CLASS_CONTENT_H_

#include "SyntaxTree_Attribute.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ClassContent : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            Function,
            Declare
        };
        
        static SyntaxTree_Attribute::Type defaultAttribute;
    public:
        SyntaxTree_ClassContent(const SyntaxTree_Base& content);
        SyntaxTree_ClassContent(SyntaxTree_Attribute* pAttribute, const SyntaxTree_Type& content);
        virtual ~SyntaxTree_ClassContent();
        
        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_ClassContent"; }
        
        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassContent*, &x);
#endif
            if (_type != dynamic_cast<const SyntaxTree_ClassContent*>(&x)->_type) return false;
            if (_type == Function) return content == dynamic_cast<const SyntaxTree_ClassContent*>(&x)->content;
            else return *pAttribute == *dynamic_cast<const SyntaxTree_ClassContent*>(&x)->pAttribute &&
                        content == dynamic_cast<const SyntaxTree_ClassContent*>(&x)->content;
        }
        
        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassContent*, &x);
#endif
            if (_type != dynamic_cast<const SyntaxTree_ClassContent*>(&x)->_type) return true;
            if (_type == Function) return content != dynamic_cast<const SyntaxTree_ClassContent*>(&x)->content;
            else return *pAttribute != *dynamic_cast<const SyntaxTree_ClassContent*>(&x)->pAttribute ||
                        content != dynamic_cast<const SyntaxTree_ClassContent*>(&x)->content;
        }
    protected:
        SyntaxTree_Attribute*  pAttribute;
        const SyntaxTree_Base& content;
        Type                   _type;
    };
}

#endif
