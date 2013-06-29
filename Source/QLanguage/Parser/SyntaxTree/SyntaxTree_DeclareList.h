/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   16:22
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_DeclareList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_DeclareName
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_DECLARE_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_DECLARE_LIST_H_

#include "SyntaxTree_ArrayLst.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Exp;

    class SyntaxTree_DeclareList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_DeclareList(const SyntaxTree_Type& type);
        virtual ~SyntaxTree_DeclareList();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_DeclareList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_DeclareList*, &x);
#endif
            return _type == dynamic_cast<const SyntaxTree_DeclareList*>(&x)->_type && childs.size() == x.childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_DeclareList*, &x);
#endif
            return _type != dynamic_cast<const SyntaxTree_DeclareList*>(&x)->_type || childs.size() != x.childs.size();
        }
    protected:
        const SyntaxTree_Type& _type;
    };
}

#endif
