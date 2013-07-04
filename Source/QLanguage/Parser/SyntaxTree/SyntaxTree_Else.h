/********************************************************************
	created:	2013/07/04
	created:	4:7:2013   22:00
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Else.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Else
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_ELSE_H_
#define _QLANGUAGE_SYNTAX_TREE_ELSE_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Else : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Else(const SyntaxTree_Base& content);
        virtual ~SyntaxTree_Else();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_Else"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Else*, &x);
#endif
            return content == dynamic_cast<const SyntaxTree_Else*>(&x)->content;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Else*, &x);
#endif
            return content != dynamic_cast<const SyntaxTree_Else*>(&x)->content;
        }
    protected:
        const SyntaxTree_Base& content;
    };
}

#endif
