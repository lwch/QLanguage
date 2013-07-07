/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   15:35
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Break.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Break
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_BREAK_H_
#define _QLANGUAGE_SYNTAX_TREE_BREAK_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Break : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Break();
        virtual ~SyntaxTree_Break();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_Break"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Break*, &x);
#endif
            return true;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Break*, &x);
#endif
            return false;
        }
    };
}

#endif
