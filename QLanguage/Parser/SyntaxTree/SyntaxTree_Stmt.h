/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:13
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Stmt.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Stmt
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_STMT_H_
#define _QLANGUAGE_SYNTAX_TREE_STMT_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Stmt : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            Assign,
            Call,
            Declare,
            Do,
            If,
            For,
            While,
            Return
        };
    public:
        SyntaxTree_Stmt(SyntaxTree_Base* pStmt, Type type);
        virtual ~SyntaxTree_Stmt();
        
        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Stmt"; }
        
        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Stmt*, &x);
#endif
            return *pStmt == *dynamic_cast<const SyntaxTree_Stmt*>(&x)->pStmt;
        }
        
        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Stmt*, &x);
#endif
            return *pStmt != *dynamic_cast<const SyntaxTree_Stmt*>(&x)->pStmt;
        }
    protected:
        SyntaxTree_Base* pStmt;
        Type             _type;
    };
}

#endif
