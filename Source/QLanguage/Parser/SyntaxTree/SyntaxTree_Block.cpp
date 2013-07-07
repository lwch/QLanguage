/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   20:40
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Block.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Block
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_StmtList.h"
#include "SyntaxTree_Block.h"

namespace QLanguage
{
    SyntaxTree_Block::SyntaxTree_Block() : parent(sizeof(SyntaxTree_Block)), pStmts(NULL)
    {
    }

    SyntaxTree_Block::SyntaxTree_Block(SyntaxTree_Base* pStmts) : parent(sizeof(SyntaxTree_Block)), pStmts(pStmts)
    {
    }

    SyntaxTree_Block::~SyntaxTree_Block()
    {
    }

    void SyntaxTree_Block::print(ostream& stream, uint indent)const
    {
        this->printIndent(stream, indent - parent::indent);
        stream << '{' << endl;
        if (pStmts) pStmts->print(stream, indent);
        this->printIndent(stream, indent - parent::indent);
        stream << '}';
    }

    // block -> "{" stmt_list "}"
    bool Parser::reduceBlockStmts()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_StmtList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Block* pBlock = allocator<SyntaxTree_Block>::allocate();
        construct(pBlock, syntaxTreeStack.top());

        context.data.insert(pBlock);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pBlock);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // block -> "{" "}"
    bool Parser::reduceBlockEmpty()
    {
        SyntaxTree_Block* pBlock = allocator<SyntaxTree_Block>::allocate();
        construct(pBlock);

        context.data.insert(pBlock);

        syntaxTreeStack.push(pBlock);

        shifts.pop();
        shifts.pop();

        return true;
    }
}
