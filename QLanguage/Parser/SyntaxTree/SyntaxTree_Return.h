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
        SyntaxTree_Return(SyntaxTree_Base* pExp);
        virtual ~SyntaxTree_Return();

        virtual void print(ostream& stream, uint indent)const;
    protected:
        SyntaxTree_Base* pExp;
    };
}

#endif
