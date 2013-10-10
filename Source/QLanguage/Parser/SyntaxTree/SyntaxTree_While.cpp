/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:13
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_While.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_While
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"

#include "SyntaxTree_Ext.h"

#include "SyntaxTree_While.h"

namespace QLanguage
{
    SyntaxTree_While::SyntaxTree_While(SyntaxTree_Exp& exp, SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_While))
        , exp(exp)
        , block(block)
    {
    }
    
    SyntaxTree_While::~SyntaxTree_While()
    {
    }
    
    void SyntaxTree_While::print(ostream& stream, uint indent)const
    {
        stream << "while (";
        exp.print(stream, indent);
        stream << ") ";
        block.print(stream, indent + parent::indent);
    }

    bool SyntaxTree_While::make(Parser *pParser)
    {
        return make_exp(pParser, exp,
            [this](Parser* pParser) { // 常量值true，死循环
                size_t ri = pParser->instructions.size() - 1;
                if (!block.make(pParser)) return false;
                VM::Instruction i;
                i.op = VM::OpCode::Jmp;
                i.ot = MAKE_OT(0, 0, 0);
                i.Jmp.ext = false;
                i.Jmp.addr = ri - pParser->instructions.size();
                pParser->instructions.push_back(i);
                return true;
            },
            [this](Parser* pParser) { // 常量值false，不生成block的代码
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
                pParser->instructions[ri].Jmp.addr = pParser->instructions.size() - ri - 1;
                return true;
            });
    }

    template <typename T1, typename T2, typename T3>
    inline void construct(T1* p, T2& v1, T3& v2)
    {
        new (p) T1(v1, v2);
    }
    
    // while_desc -> "while" "(" exp ")" block
    bool Parser::reduceWhile()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();
        
        SyntaxTree_While* pWhile = allocator<SyntaxTree_While>::allocate();
        construct(pWhile, dynamic_cast<SyntaxTree_Exp&>(*syntaxTreeStack[1]), dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pWhile);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pWhile);
        return true;
    }
}
