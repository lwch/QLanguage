/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   15:44
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Continue.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Continue
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_CONTINUE_H_
#define _QLANGUAGE_SYNTAX_TREE_CONTINUE_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Continue : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Continue();
        virtual ~SyntaxTree_Continue();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_Continue"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Continue*, &x);
#endif
            return true;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Continue*, &x);
#endif
            return false;
        }
    };
}

#endif
