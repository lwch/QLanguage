/********************************************************************
	created:	2013/06/13
	created:	13:6:2013   16:02
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Assign.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Assign
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Assign.h"

namespace QLanguage
{
    SyntaxTree_Assign::SyntaxTree_Assign(SyntaxTree_MemberList& memberList, SyntaxTree_Base& exp, Type type)
        : parent(sizeof(SyntaxTree_Assign))
        , memberList(memberList)
        , exp(exp)
        , type(type)
    {
    }

    SyntaxTree_Assign::~SyntaxTree_Assign()
    {
    }

    void SyntaxTree_Assign::print(ostream& stream, uint indent)const
    {
    }

    // TODO

    // assign_desc -> member_desc "+" "=" exp
    bool Parser::reduceAssignAddEqual()
    {
//         SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
//         construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::AddEqual);
        return true;
    }

    // assign_desc -> member_desc "-" "=" exp
    bool Parser::reduceAssignSubEqual()
    {
        return true;
    }

    // assign_desc -> member_desc "&" "=" exp
    bool Parser::reduceAssignAndEqual()
    {
        return true;
    }

    // assign_desc -> member_desc "|" "=" exp
    bool Parser::reduceAssignOrEqual()
    {
        return true;
    }

    // assign_desc -> member_desc "^" "=" exp
    bool Parser::reduceAssignXorEqual()
    {
        return true;
    }

    // assign_desc -> member_desc "<" "<" "=" exp
    bool Parser::reduceAssignLeftMoveEqual()
    {
        return true;
    }

    // assign_desc -> member_desc ">" ">" "=" exp
    bool Parser::reduceAssignRightMoveEqual()
    {
        return true;
    }

    // assign_desc -> member_desc "=" exp
    bool Parser::reduceAssignEqual()
    {
        return true;
    }
}
