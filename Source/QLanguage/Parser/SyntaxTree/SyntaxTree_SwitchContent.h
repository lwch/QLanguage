/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   14:24
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_SwitchContent.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_SwitchContent
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_SWITCH_CONTENT_H_
#define _QLANGUAGE_SYNTAX_TREE_SWITCH_CONTENT_H_

#include "SyntaxTree_Exp.h"
#include "SyntaxTree_StmtList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_SwitchContent : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_SwitchContent(SyntaxTree_Exp* pExp, const SyntaxTree_StmtList& stmtList);
        SyntaxTree_SwitchContent(const SyntaxTree_StmtList& stmtList);
        virtual ~SyntaxTree_SwitchContent();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_SwitchContent"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_SwitchContent*, &x);
#endif
            return parent::checkEqual(pExp, dynamic_cast<const SyntaxTree_SwitchContent*>(&x)->pExp) &&
                   stmtList == dynamic_cast<const SyntaxTree_SwitchContent*>(&x)->stmtList;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_SwitchContent*, &x);
#endif
            return parent::checkNotEqual(pExp, dynamic_cast<const SyntaxTree_SwitchContent*>(&x)->pExp) ||
                   stmtList != dynamic_cast<const SyntaxTree_SwitchContent*>(&x)->stmtList;
        }
    protected:
        SyntaxTree_Exp* pExp;
        const SyntaxTree_StmtList& stmtList;
    };
}

#endif
