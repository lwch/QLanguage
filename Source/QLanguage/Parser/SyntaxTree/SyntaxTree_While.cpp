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
#include "SyntaxTree_While.h"

namespace QLanguage
{
    SyntaxTree_While::SyntaxTree_While(const SyntaxTree_Exp& exp, const SyntaxTree_Block& block)
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
    }
    
    // while_desc -> "while" "(" exp ")" block
    bool Parser::reduceWhile()
    {
        shifts.pop();
        shifts.pop();
        shifts.pop();
        
        SyntaxTree_While* pWhile = allocator<SyntaxTree_While>::allocate();
        construct(pWhile, dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack[1]), dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pWhile);
        return true;
    }
}
