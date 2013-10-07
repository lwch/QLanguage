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
    SyntaxTree_If::SyntaxTree_If(SyntaxTree_Exp& exp, SyntaxTree_Base& op1, SyntaxTree_Else* pElse)
        : parent(sizeof(SyntaxTree_If))
        , exp(exp)
        , op1(op1)
        , pElse(pElse)
    {
    }

    SyntaxTree_If::SyntaxTree_If(SyntaxTree_Exp& exp, SyntaxTree_Base& op1)
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
        stream << ")";
        if (op1.type() == "SyntaxTree_Stmt")
        {
            stream << ' ';
            op1.print(stream, 0);
            stream << endl;
            this->printIndent(stream, indent);
        }
        else
        {
            stream << endl;
            op1.print(stream, indent + parent::indent);
            stream << ' ';
        }
        if (pElse) pElse->print(stream, indent);
    }

    bool SyntaxTree_If::make(Parser* pParser)
    {
        if (!exp.make(pParser)) return false;
        if (exp.isConstValue())
        {
            if (exp.toVariant(pParser).toBool())
            {
                return op1.make(pParser);
            }
            else if (pElse)
            {
                return pElse->make(pParser);
            }
        }
        else
        {
            // | exp                |
            // | jmp lb1 when false |
            // | op1                |
            // | jmp lb2            |
            // | lb1:               |
            // | [else]             |
            // | lb2:               |
            // | ...                |
            VM::Instruction i;
            i.op = VM::OpCode::Jmp;
            i.ot = MAKE_OT(0, 0, 0);
            i.Jmp.ext = true;
            i.Jmp.ob  = 0;
            i.Jmp.os  = 65534;
            pParser->instructions.push_back(i); // 判断表达式是否为false并跳转
            size_t ri = pParser->instructions.size() - 1; // 地址需回填
            if (!op1.make(pParser)) return false;

            if (pElse)
            {
                i.Jmp.ext = false;
                pParser->instructions.push_back(i); // true条件的代码执行完毕，跳转到else的代码之后
                size_t r = pParser->instructions.size() - 1; // 地址需回填
                if (!pElse->make(pParser)) return false;
                pParser->instructions[r].Jmp.addr = pParser->instructions.size() - r - 1;
            }
            pParser->instructions[ri].Jmp.addr = pParser->instructions.size() - ri - 1;
        }
        return true;
    }

    template <typename T1, typename T2, typename T3>
    inline void construct(T1* p, T2& v1, T3& v2)
    {
        new (p) T1(v1, v2);
    }

    template <typename T1, typename T2, typename T3, typename T4>
    inline void construct(T1* p, T2& v1, T3& v2, const T4& v3)
    {
        new (p) T1(v1, v2, v3);
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
        construct(pIf, dynamic_cast<SyntaxTree_Exp&>(*syntaxTreeStack[2]), *syntaxTreeStack[1], dynamic_cast<SyntaxTree_Else*>(syntaxTreeStack.top()));

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
        construct(pIf, dynamic_cast<SyntaxTree_Exp&>(*syntaxTreeStack[1]), *syntaxTreeStack.top());

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
        construct(pIf, dynamic_cast<SyntaxTree_Exp&>(*syntaxTreeStack[2]), *syntaxTreeStack[1], dynamic_cast<SyntaxTree_Else*>(syntaxTreeStack.top()));

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
        construct(pIf, dynamic_cast<SyntaxTree_Exp&>(*syntaxTreeStack[1]), *syntaxTreeStack.top());

        context.data.insert(pIf);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pIf);

        return true;
    }
}
