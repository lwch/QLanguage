/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   14:35
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_DeclareName.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_DeclareName
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_DECLARE_NAME_H_
#define _QLANGUAGE_SYNTAX_TREE_DECALRE_NAME_H_

#include "SyntaxTree_ArrayLst.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Exp;

    class SyntaxTree_DeclareName : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_DeclareName(const string& name, SyntaxTree_ArrayLst* pArrayLst, SyntaxTree_Exp* pExp);
        SyntaxTree_DeclareName(const string& name, SyntaxTree_ArrayLst* pArrayLst);
        SyntaxTree_DeclareName(const string& name, SyntaxTree_Exp* pExp);
        SyntaxTree_DeclareName(const string& name);
        virtual ~SyntaxTree_DeclareName();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_DeclareName"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_DeclareName*, &x);
#endif
            return name == dynamic_cast<const SyntaxTree_DeclareName*>(&x)->name &&
                ((pArrayLst && dynamic_cast<const SyntaxTree_DeclareName*>(&x)->pArrayLst && *pArrayLst == *dynamic_cast<const SyntaxTree_DeclareName*>(&x)->pArrayLst) ||
                (pArrayLst == NULL && dynamic_cast<const SyntaxTree_DeclareName*>(&x) == NULL));
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_DeclareName*, &x);
#endif
            return name != dynamic_cast<const SyntaxTree_DeclareName*>(&x)->name ||
                ((pArrayLst && (dynamic_cast<const SyntaxTree_DeclareName*>(&x)->pArrayLst == NULL || *pArrayLst != *dynamic_cast<const SyntaxTree_DeclareName*>(&x)->pArrayLst)) ||
                (pArrayLst == NULL && dynamic_cast<const SyntaxTree_DeclareName*>(&x)->pArrayLst));
        }
    protected:
        string               name;
        SyntaxTree_ArrayLst* pArrayLst;
        SyntaxTree_Exp*      pExp;
    };
}

#endif
