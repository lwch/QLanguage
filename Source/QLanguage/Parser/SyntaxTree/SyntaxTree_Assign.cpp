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
#include "SyntaxTree_Exp.h"
#include "SyntaxTree_Assign.h"

namespace QLanguage
{
    SyntaxTree_Assign::SyntaxTree_Assign(const SyntaxTree_MemberList& memberList, const SyntaxTree_Base& exp, Type type)
        : parent(sizeof(SyntaxTree_Assign))
        , _type(type)
        , memberList(memberList)
        , exp(exp)
    {
    }

    SyntaxTree_Assign::~SyntaxTree_Assign()
    {
    }

    void SyntaxTree_Assign::print(ostream& stream, uint indent)const
    {
        memberList.print(stream, indent);
        switch (_type)
        {
        case AddEqual:
            stream << " += ";
            break;
        case SubEqual:
            stream << " -= ";
            break;
        case AndEqual:
            stream << " &= ";
            break;
        case OrEqual:
            stream << " |= ";
            break;
        case XorEqual:
            stream << " ^= ";
            break;
        case LeftMoveEqual:
            stream << " <<= ";
            break;
        case RightMoveEqual:
            stream << " >>= ";
            break;
        case Equal:
            stream << " = ";
            break;
        }
        exp.print(stream, indent);
    }

    // assign_desc -> member_desc "+" "=" exp
    bool Parser::reduceAssignAddEqual()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList* , syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
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
