/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   14:36
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ClassInherit.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ClassInherit
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_CLASS_INHERIT_H_
#define _QLANGUAGE_SYNTAX_TREE_CLASS_INHERIT_H_

#include "SyntaxTree_Attribute.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ClassInherit : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            Implement,
            Extend
        };
    public:
        SyntaxTree_ClassInherit(SyntaxTree_Attribute& attribute, Type type, const string& name);
        virtual ~SyntaxTree_ClassInherit();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassInherit*, &x);
#endif
            return attribute == dynamic_cast<const SyntaxTree_ClassInherit*>(&x)->attribute &&
                   type == dynamic_cast<const SyntaxTree_ClassInherit*>(&x)->type &&
                   name == dynamic_cast<const SyntaxTree_ClassInherit*>(&x)->name;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassInherit*, &x);
#endif
            return attribute != dynamic_cast<const SyntaxTree_ClassInherit*>(&x)->attribute ||
                   type != dynamic_cast<const SyntaxTree_ClassInherit*>(&x)->type ||
                   name != dynamic_cast<const SyntaxTree_ClassInherit*>(&x)->name;
        }
    protected:
        SyntaxTree_Attribute& attribute;
        Type                  type;
        string                name;
    };
}

#endif
