/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   11:28
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Values.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Values
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_VALUES_H_
#define _QLANGUAGE_SYNTAX_TREE_VALUES_H_

#include "SyntaxTree_ValuesList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Values : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Values(const SyntaxTree_ValuesList& valuesList);
        virtual ~SyntaxTree_Values();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_Values"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Values*, &x);
#endif
            return valuesList == dynamic_cast<const SyntaxTree_Values*>(&x)->valuesList;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Values*, &x);
#endif
            return valuesList != dynamic_cast<const SyntaxTree_Values*>(&x)->valuesList;
        }
    protected:
        const SyntaxTree_ValuesList& valuesList;
    };
}

#endif
