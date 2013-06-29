/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   11:27
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ArrayValue.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ArrayValue
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_SYNTAX_TREE_ARRAYVALUE_H_
#define _QLANGUAGE_SYNTAX_TREE_ARRAYVALUE_H_

#include "SyntaxTree_Values.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ArrayValue : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ArrayValue(SyntaxTree_Values* pValues);
        virtual ~SyntaxTree_ArrayValue();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_ArrayValue"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ArrayValue*, &x);
#endif
            return *pValues == *dynamic_cast<const SyntaxTree_ArrayValue*>(&x)->pValues;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ArrayValue*, &x);
#endif
            return *pValues != *dynamic_cast<const SyntaxTree_ArrayValue*>(&x)->pValues;
        }
    protected:
        SyntaxTree_Values* pValues;
    };
}

#endif
