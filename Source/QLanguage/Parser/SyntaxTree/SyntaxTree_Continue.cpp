/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   15:47
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Continue.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Continue
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Continue.h"

namespace QLanguage
{
    SyntaxTree_Continue::SyntaxTree_Continue()
        : parent(sizeof(SyntaxTree_Continue))
    {
    }

    SyntaxTree_Continue::~SyntaxTree_Continue()
    {
    }

    void SyntaxTree_Continue::print(ostream& stream, uint indent)const
    {
        stream << "continue";
    }

    // continue_desc -> "continue"
    bool Parser::reduceContinue()
    {
        shifts.pop();

        SyntaxTree_Continue* pContinue = allocator<SyntaxTree_Continue>::allocate();
        construct(pContinue);

        context.data.insert(pContinue);

        syntaxTreeStack.push(pContinue);

        return true;
    }
}
