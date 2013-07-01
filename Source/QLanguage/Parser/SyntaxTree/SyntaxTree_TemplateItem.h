/********************************************************************
	created:	2013/07/01
	created:	1:7:2013   21:13
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_TemplateItem.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_TemplateItem
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_TEMPLATE_ITEM_H_
#define _QLANGUAGE_SYNTAX_TREE_TEMPLATE_ITEM_H_

#include "SyntaxTree_Type.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_TemplateItem : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            _TypeName,
            _Type
        };
    public:
        SyntaxTree_TemplateItem(const string& name);
        SyntaxTree_TemplateItem(SyntaxTree_Type* pType, const string& name);
        virtual ~SyntaxTree_TemplateItem();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_TemplateItem"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_TemplateItem*, &x);
#endif
            if (_type != dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->_type) return false;
            return ((pType && dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->pType && *pType == *dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->pType) ||
                    (pType == NULL && dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->pType == NULL));
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_TemplateItem*, &x);
#endif
            if (_type != dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->_type) return true;
            return ((pType && (dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->pType == NULL || *pType != *dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->pType)) ||
                    (pType == NULL && dynamic_cast<const SyntaxTree_TemplateItem*>(&x)->pType != NULL));
        }
    protected:
        SyntaxTree_Type* pType;
        Type             _type;
        string           name;
    };
}

#endif
