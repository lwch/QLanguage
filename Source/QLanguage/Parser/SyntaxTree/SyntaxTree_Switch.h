/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   15:07
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Switch.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Switch
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_SWITCH_H_
#define _QLANGUAGE_SYNTAX_TREE_SWITCH_H_

#include "SyntaxTree_Exp.h"
#include "SyntaxTree_SwitchContentList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Switch : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Switch(const SyntaxTree_Exp& exp, const SyntaxTree_SwitchContentList& contentList);
        virtual ~SyntaxTree_Switch();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_Switch"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Switch*, &x);
#endif
            return exp == dynamic_cast<const SyntaxTree_Switch*>(&x)->exp &&
                   contentList == dynamic_cast<const SyntaxTree_Switch*>(&x)->contentList;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Switch*, &x);
#endif
            return exp != dynamic_cast<const SyntaxTree_Switch*>(&x)->exp ||
                   contentList != dynamic_cast<const SyntaxTree_Switch*>(&x)->contentList;
        }
    protected:
        const SyntaxTree_Exp& exp;
        const SyntaxTree_SwitchContentList& contentList;
    };
}

#endif
