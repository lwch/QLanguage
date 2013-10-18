/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:10
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Do.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Do
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Ext.h"
#include "SyntaxTree_Do.h"

namespace QLanguage
{
    SyntaxTree_Do::SyntaxTree_Do(SyntaxTree_Block& block, SyntaxTree_Exp& exp)
        : parent(sizeof(SyntaxTree_Do))
        , block(block)
        , exp(exp)
    {
    }

    SyntaxTree_Do::~SyntaxTree_Do()
    {
    }

    void SyntaxTree_Do::print(ostream& stream, uint indent)const
    {
        stream << "do ";
        block.print(stream, indent + parent::indent);
        stream << " while (";
        exp.print(stream, indent);
        stream << ")";
    }

    bool SyntaxTree_Do::make(Parser *pParser)
    {
        size_t ri = pParser->instructions.size() - 1;
        if (!block.make(pParser)) return false;
        return make_exp(pParser, exp,
            [this, ri](Parser* pParser) { // 常量值true，死循环
                VM::Instruction i;
                i.op = VM::OpCode::Jmp;
                i.ot = MAKE_OT(0, 0, 0);
                i.Jmp.ext  = false;
                i.Jmp.addr = ri - pParser->instructions.size();
                pParser->instructions.push_back(i);
                return true;
            },
            [this](Parser* pParser) { // 常量值false，只运行一次
                return true;
            },
            [this, ri](Parser* pParser) {
                VM::Instruction i;
                i.op = VM::OpCode::Jmp;
                i.ot = MAKE_OT(0, 0, 0);
                i.Jmp.ext  = true;
                i.Jmp.addr = 1;
                i.Jmp.ob   = 0;
                i.Jmp.os   = 65534;
                pParser->instructions.push_back(i);
                i.Jmp.ext  = false;
                i.Jmp.addr = ri - pParser->instructions.size();
                pParser->instructions.push_back(i);
                return true;
            });
    }

    template <typename T1, typename T2, typename T3>
    inline void construct(T1* p, T2& v1, T3& v2)
    {
        new (p) T1(v1, v2);
    }

    // do_desc -> "do" block "while" "(" exp ")"
    bool Parser::reduceDo()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();
        shifts.pop();
        
        SyntaxTree_Do* pDo = allocator<SyntaxTree_Do>::allocate();
        construct(pDo, dynamic_cast<SyntaxTree_Block&>(*syntaxTreeStack[1]), dynamic_cast<SyntaxTree_Exp&>(*syntaxTreeStack.top()));

        context.data.insert(pDo);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pDo);
        return true;
    }
}
