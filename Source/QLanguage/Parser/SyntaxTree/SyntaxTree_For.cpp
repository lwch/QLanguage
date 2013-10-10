/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:11
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_For.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_For
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"

#include "SyntaxTree_Ext.h"

#include "SyntaxTree_Stmt.h"
#include "SyntaxTree_For.h"

namespace QLanguage
{
    SyntaxTree_For::SyntaxTree_For(SyntaxTree_Base& stmt1, SyntaxTree_Exp& exp, SyntaxTree_Base& stmt2, SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_For))
        , stmt1(stmt1)
        , exp(exp)
        , stmt2(stmt2)
        , block(block)
    {

    }

    SyntaxTree_For::~SyntaxTree_For()
    {

    }

    void SyntaxTree_For::print(ostream& stream, uint indent) const
    {
        stream << "for (";
        stmt1.print(stream, 0);
        stream << "; ";
        exp.print(stream, indent);
        stream << "; ";
        stmt2.print(stream, 0);
        stream << ")" << endl;
        this->printIndent(stream, indent);
        block.print(stream, indent + parent::indent);
    }

    bool SyntaxTree_For::make(Parser *pParser)
    {
        if (!stmt1.make(pParser)) return false;
        return make_exp(pParser, exp,
            [this](Parser* pParser) { // 常量值true，死循环
                size_t ri = pParser->instructions.size() - 1;
                if (!block.make(pParser)) return false;
                if (!stmt2.make(pParser)) return false;
                VM::Instruction i;
                i.op = VM::OpCode::Jmp;
                i.ot = MAKE_OT(0, 0, 0);
                i.Jmp.ext = false;
                i.Jmp.addr = ri - pParser->instructions.size();
                pParser->instructions.push_back(i);
                return true;
            },
            [this](Parser* pParser) { // 常量值false，不生成block和stmt2的代码
                return true;
            },
            [this](Parser* pParser) {
                VM::Instruction i;
                i.op = VM::OpCode::Jmp;
                i.ot = MAKE_OT(0, 0, 0);
                i.Jmp.ext = true;
                i.Jmp.ob = 0;
                i.Jmp.os = 65534;
                pParser->instructions.push_back(i); // 判断表达式是否为false并跳转
                size_t ri = pParser->instructions.size() - 1; // 地址需回填
                if (!block.make(pParser)) return false;
                if (!stmt2.make(pParser)) return false;
                pParser->instructions[ri].Jmp.addr = pParser->instructions.size() - ri - 1;
                return true;
            });
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    inline void construct(T1* p, T2& v1, T3& v2, T4& v3, T5& v4)
    {
        new (p) T1(v1, v2, v3, v4);
    }

    // for_desc -> "for" "(" stmt_no_semicolon ";" exp ";" stmt_no_semicolon ")" block
    bool Parser::reduceFor()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Stmt*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Stmt*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();
        shifts.pop();
        shifts.pop();
	
        SyntaxTree_For* pFor = allocator<SyntaxTree_For>::allocate();
        construct(pFor,
                 *syntaxTreeStack[3],
                  dynamic_cast<SyntaxTree_Exp&>(*syntaxTreeStack[2]),
                 *syntaxTreeStack[1],
                  dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));
    
        context.data.insert(pFor);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFor);
    
        return true;
    }
}
