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
    SyntaxTree_Assign::SyntaxTree_Assign(SyntaxTree_MemberList& memberList, SyntaxTree_Base& exp, Type type)
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

    bool SyntaxTree_Assign::make(Parser* pParser)
    {
        pair<short, ushort> p = pParser->indexOfRegister(memberList.name());
        if (p.first == -1)
        {
            if (memberList.childs.size() > 1) // TODO: xxx.xxx...
            {
            }
            else
            {
                string str = string::format("undefined variable %s", memberList.name());
                throw error<const char*>(str.c_str(), __FILE__, __LINE__);
            }
        }
        if (!exp.make(pParser)) return false;
        VM::Instruction i;
        i.ot = MAKE_OT(0, exp.isConstValue(), 0);
        i.Normal.ob1 = i.Normal.obd = (uchar)p.first;
        i.Normal.os1 = i.Normal.od  = p.second;
        i.Normal.ob2 = exp.getBlock();
        i.Normal.os2 = exp.getIndex();
        switch (_type)
        {
        case AddEqual:
            i.op = VM::OpCode::Add;
            break;
        case SubEqual:
            i.op = VM::OpCode::Sub;
            break;
        case AndEqual:
            i.op = VM::OpCode::BitAnd;
            break;
        case OrEqual:
            i.op = VM::OpCode::BitOr;
            break;
        case XorEqual:
            i.op = VM::OpCode::BitXor;
            break;
        case LeftMoveEqual: // TODO
            break;
        case RightMoveEqual: // TODO
            break;
        case Equal:
            i.op = VM::OpCode::Mov;
            i.Normal.ob1 = exp.getBlock();
            i.Normal.os1 = exp.getIndex();
            break;
        default:
            throw error<const char*>("can't do here", __FILE__, __LINE__);
            return false;
        }
        pParser->instructions.push_back(i);
        return true;
    }

    template <typename T1, typename T2, typename T3, typename T4>
    inline void construct(T1* p, T2& v1, T3& v2, const T4& v3)
    {
        new (p) T1(v1, v2, v3);
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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::AddEqual);

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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::SubEqual);

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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::AndEqual);

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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::OrEqual);

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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::XorEqual);

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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::LeftMoveEqual);

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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::RightMoveEqual);

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
        construct(pAssign, dynamic_cast<SyntaxTree_MemberList&>(*syntaxTreeStack[1]), *syntaxTreeStack.top(), SyntaxTree_Assign::Equal);

        context.data.insert(pAssign);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pAssign);
        return true;
    }
}
