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
#include "SyntaxTree_Do.h"

namespace QLanguage
{
    SyntaxTree_Do::SyntaxTree_Do(const SyntaxTree_Block& block, const SyntaxTree_Exp& exp)
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
        construct(pDo, dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack[1]), dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack.top()));
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pDo);
        return true;
    }
}
