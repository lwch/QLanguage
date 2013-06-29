/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   16:23
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Paramter.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Paramter
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_PARAMTER_H_
#define _QLANGUAGE_SYNTAX_TREE_PARAMTER_H_

#include "SyntaxTree_ArrayLst.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Type;

    class SyntaxTree_Paramter : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Paramter(const SyntaxTree_Type& type, const string& name, SyntaxTree_ArrayLst* pArrayLst);
        SyntaxTree_Paramter(const SyntaxTree_Type& type, const string& name);
        SyntaxTree_Paramter(const SyntaxTree_Type& type);
        virtual ~SyntaxTree_Paramter();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Paramter"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Paramter*, &x);
#endif
            return _type == dynamic_cast<const SyntaxTree_Paramter*>(&x)->_type &&
                   ((pArrayLst && dynamic_cast<const SyntaxTree_Paramter*>(&x)->pArrayLst && *pArrayLst == *dynamic_cast<const SyntaxTree_Paramter*>(&x)->pArrayLst) ||
                    (pArrayLst == NULL && dynamic_cast<const SyntaxTree_Paramter*>(&x)->pArrayLst == NULL));
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Paramter*, &x);
#endif
            return _type != dynamic_cast<const SyntaxTree_Paramter*>(&x)->_type &&
                   ((pArrayLst && (dynamic_cast<const SyntaxTree_Paramter*>(&x)->pArrayLst == NULL || *pArrayLst != *dynamic_cast<const SyntaxTree_Paramter*>(&x)->pArrayLst)) ||
                    (pArrayLst == NULL && dynamic_cast<const SyntaxTree_Paramter*>(&x)->pArrayLst));
        }
    protected:
        const SyntaxTree_Type& _type;
        string                 name;
        SyntaxTree_ArrayLst*   pArrayLst;
    };
}

#endif
