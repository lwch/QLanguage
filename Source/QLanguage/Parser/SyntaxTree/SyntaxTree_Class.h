/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   13:33
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Class.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Class
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_CLASS_H_
#define _QLANGUAGE_SYNTAX_TREE_CLASS_H_

#include "SyntaxTree_Template.h"
#include "SyntaxTree_ClassContentList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ClassName;
    class SyntaxTree_ClassInherit;

    class SyntaxTree_Class : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit, SyntaxTree_ClassContentList* pContentList);
        SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit, SyntaxTree_ClassContentList* pContentList);
        SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name, SyntaxTree_ClassContentList* pContentList);
        SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassContentList* pContentList);
        SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit);
        SyntaxTree_Class(const SyntaxTree_ClassName& name, SyntaxTree_ClassInherit* pInherit);
        SyntaxTree_Class(SyntaxTree_Template* pTemplate, const SyntaxTree_ClassName& name);
        SyntaxTree_Class(const SyntaxTree_ClassName& name);
        virtual ~SyntaxTree_Class();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Class"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Class*, &x);
#endif
            return parent::checkEqual(pTemplate, dynamic_cast<const SyntaxTree_Class*>(&x)->pTemplate) &&
                   parent::checkEqual(pContentList, dynamic_cast<const SyntaxTree_Class*>(&x)->pContentList);
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Class*, &x);
#endif
            return parent::checkNotEqual(pTemplate, dynamic_cast<const SyntaxTree_Class*>(&x)->pTemplate) ||
                   parent::checkNotEqual(pContentList, dynamic_cast<const SyntaxTree_Class*>(&x)->pContentList);
        }
    protected:
        SyntaxTree_Template*         pTemplate;
        const SyntaxTree_ClassName&  name;
        SyntaxTree_ClassInherit*     pInherit;
        SyntaxTree_ClassContentList* pContentList;
    };
}

#endif
