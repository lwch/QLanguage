/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   21:11
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_StmtList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_StmtList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_StmtList.h"

namespace QLanguage
{
    SyntaxTree_StmtList::SyntaxTree_StmtList() : parent(sizeof(SyntaxTree_StmtList))
    {
    }

    SyntaxTree_StmtList::~SyntaxTree_StmtList()
    {
    }

    void SyntaxTree_StmtList::print(ostream& stream, uint indent)const
    {
        for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(); i != m; ++i)
        {
            (*i)->print(stream, indent);
            stream << endl;
        }
    }

    // stmt_list -> stmt_list stmt
    // stmt_list -> stmt_list block
    bool Parser::reduceStmtList2Size()
    {
        SyntaxTree_StmtList* pStmtList = dynamic_cast<SyntaxTree_StmtList*>(syntaxTreeStack[1]);

        pStmtList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        return true;
    }

    // stmt_list -> stmt
    // stmt_list -> block
    bool Parser::reduceStmtList1Size()
    {
        SyntaxTree_StmtList* pStmtList = allocator<SyntaxTree_StmtList>::allocate();
        construct(pStmtList);

        pStmtList->pushChild(syntaxTreeStack.top());

        context.data.insert(pStmtList);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pStmtList);
        return true;
    }
}
