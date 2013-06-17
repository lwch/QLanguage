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
    SyntaxTree_Assign::SyntaxTree_Assign(const SyntaxTree_MemberList& memberList, const SyntaxTree_Base& exp, Type type)
        : parent(sizeof(SyntaxTree_Assign))
        , type(type)
        , memberList(memberList)
        , exp(exp)
    {
    }

    SyntaxTree_Assign::~SyntaxTree_Assign()
    {
    }

    void SyntaxTree_Assign::print(ostream& stream, uint indent)const
    {
    }

    // assign_desc -> member_desc "+" "=" exp
    bool Parser::reduceAssignAddEqual()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::AddEqual);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }

    // assign_desc -> member_desc "-" "=" exp
    bool Parser::reduceAssignSubEqual()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::SubEqual);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }

    // assign_desc -> member_desc "&" "=" exp
    bool Parser::reduceAssignAndEqual()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::AndEqual);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }

    // assign_desc -> member_desc "|" "=" exp
    bool Parser::reduceAssignOrEqual()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::OrEqual);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }

    // assign_desc -> member_desc "^" "=" exp
    bool Parser::reduceAssignXorEqual()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::XorEqual);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }

    // assign_desc -> member_desc "<" "<" "=" exp
    bool Parser::reduceAssignLeftMoveEqual()
    {
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::LeftMoveEqual);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }

    // assign_desc -> member_desc ">" ">" "=" exp
    bool Parser::reduceAssignRightMoveEqual()
    {
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::RightMoveEqual);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }

    // assign_desc -> member_desc "=" exp
    bool Parser::reduceAssignEqual()
    {
        shifts.pop();

        SyntaxTree_Assign* pAssign = allocator<SyntaxTree_Assign>::allocate();
        construct(pAssign, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::Equal);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }
}
