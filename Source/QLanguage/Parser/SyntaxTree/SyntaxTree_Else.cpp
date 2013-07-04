/********************************************************************
	created:	2013/07/04
	created:	4:7:2013   22:05
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Else.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Else
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Stmt.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_Else.h"

namespace QLanguage
{
    SyntaxTree_Else::SyntaxTree_Else(const SyntaxTree_Base& content)
        : parent(sizeof(SyntaxTree_Else))
        , content(content)
    {
    }

    SyntaxTree_Else::~SyntaxTree_Else()
    {
    }

    void SyntaxTree_Else::print(ostream& stream, uint indent)const
    {
        stream << "else ";
        content.print(stream, indent + parent::indent);
    }

    // else_desc -> "else" stmt
    // else_desc -> "else" block
    bool Parser::reduceElse(ushort i)
    {
#ifdef _DEBUG
        if (i == ELSE_DESC_ELSE_STMT) TRY_CAST(SyntaxTree_Stmt*, syntaxTreeStack.top());
        else TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();

        SyntaxTree_Else* pElse = allocator<SyntaxTree_Else>::allocate();
        construct(pElse, *syntaxTreeStack.top());

        context.data.insert(pElse);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pElse);

        return true;
    }
}
