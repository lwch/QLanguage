/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   20:26
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Return.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Return
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_RETURN_H_
#define _QLANGUAGE_SYNTAX_TREE_RETURN_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Return : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Return(SyntaxTree_Base* pContent);
        virtual ~SyntaxTree_Return();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Return"; }

        bool virtual make(Parser* pParser);

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Return*, &x);
#endif
            return parent::checkEqual(pContent, dynamic_cast<const SyntaxTree_Return*>(&x)->pContent);
        }

        inline virtual const bool operator !=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Return*, &x);
#endif
            return parent::checkNotEqual(pContent, dynamic_cast<const SyntaxTree_Return*>(&x)->pContent);
        }
    protected:
        SyntaxTree_Base* pContent;
    };
}

#endif
