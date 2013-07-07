/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   15:41
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Break.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Break
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Break.h"

namespace QLanguage
{
    SyntaxTree_Break::SyntaxTree_Break()
        : parent(sizeof(SyntaxTree_Break))
    {
    }

    SyntaxTree_Break::~SyntaxTree_Break()
    {
    }

    void SyntaxTree_Break::print(ostream& stream, uint indent)const
    {
        stream << "break";
    }

    // break_desc -> "break"
    bool Parser::reduceBreak()
    {
        shifts.pop();

        SyntaxTree_Break* pBreak = allocator<SyntaxTree_Break>::allocate();
        construct(pBreak);

        context.data.insert(pBreak);

        syntaxTreeStack.push(pBreak);

        return true;
    }
}
