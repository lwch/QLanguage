/********************************************************************
	created:	2013/06/15
	created:	15:6:2013   18:09
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_If.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_If
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Stmt.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_If.h"

namespace QLanguage
{
    SyntaxTree_If::SyntaxTree_If(const SyntaxTree_Exp& exp, const SyntaxTree_Base& op1, SyntaxTree_Else* pElse)
        : parent(sizeof(SyntaxTree_If))
        , exp(exp)
        , op1(op1)
        , pElse(pElse)
    {
    }

    SyntaxTree_If::SyntaxTree_If(const SyntaxTree_Exp& exp, const SyntaxTree_Base& op1)
        : parent(sizeof(SyntaxTree_If))
        , exp(exp)
        , op1(op1)
        , pElse(NULL)
    {
    }

    SyntaxTree_If::~SyntaxTree_If()
    {
    }

    void SyntaxTree_If::print(ostream& stream, uint indent)const
    {
        stream << "if (";
        exp.print(stream, indent);
        stream << ") ";
        if (op1.type() == "SyntaxTree_Stmt")
        {
            op1.print(stream, 0);
            stream << endl;
            this->printIndent(stream, indent);
        }
        else
        {
            op1.print(stream, indent + parent::indent);
            stream << ' ';
        }
        if (pElse) pElse->print(stream, indent);
    }

    // if_desc -> "if" "(" exp ")" stmt else_desc
    bool Parser::reduceIfWithStmtElse()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Stmt*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Else*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_If* pIf = allocator<SyntaxTree_If>::allocate();
        construct(pIf, dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack[2]), *syntaxTreeStack[1], dynamic_cast<SyntaxTree_Else*>(syntaxTreeStack.top()));

        context.data.insert(pIf);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pIf);

        return true;
    }

    // if_desc -> "if" "(" exp ")" stmt
    bool Parser::reduceIfWithStmt()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Stmt*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_If* pIf = allocator<SyntaxTree_If>::allocate();
        construct(pIf, dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack[1]), *syntaxTreeStack.top());

        context.data.insert(pIf);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pIf);

        return true;
    }

    // if_desc -> "if" "(" exp ")" block else_desc
    bool Parser::reduceIfWithBlockElse()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Else*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_If* pIf = allocator<SyntaxTree_If>::allocate();
        construct(pIf, dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack[2]), *syntaxTreeStack[1], dynamic_cast<SyntaxTree_Else*>(syntaxTreeStack.top()));

        context.data.insert(pIf);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pIf);

        return true;
    }

    // if_desc -> "if" "(" exp ")" block
    bool Parser::reduceIfWithBlock()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_If* pIf = allocator<SyntaxTree_If>::allocate();
        construct(pIf, dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack[1]), *syntaxTreeStack.top());

        context.data.insert(pIf);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pIf);

        return true;
    }
}
