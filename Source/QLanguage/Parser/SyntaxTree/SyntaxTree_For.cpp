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
#include "SyntaxTree_For.h"

namespace QLanguage
{
    SyntaxTree_For::SyntaxTree_For(const SyntaxTree_Base& stmt1, const SyntaxTree_Exp& exp, const SyntaxTree_Base& stmt2, const SyntaxTree_Block& block)
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

    }

    // for_desc -> "for" "(" stmt ";" exp ";" stmt ")" block
    bool Parser::reduceFor()
    {
	shifts.pop();
	shifts.pop();
	shifts.pop();
	shifts.pop();
	shifts.pop();
	
	SyntaxTree_For* pFor = allocator<SyntaxTree_For>::allocate();
	construct(pFor,
		  *syntaxTreeStack[3],
	          dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack[2]),
		  *syntaxTreeStack[1],
		  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFor);
	return true;
    }
}
