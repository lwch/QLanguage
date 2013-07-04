/********************************************************************
	created:	2013/06/15
	created:	15:6:2013   17:29
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Exp.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Exp
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Call.h"
#include "SyntaxTree_Value.h"
#include "SyntaxTree_Exp.h"

namespace QLanguage
{
    SyntaxTree_Exp::SyntaxTree_Exp(const SyntaxTree_Base& OP1, SyntaxTree_Base* pOP2, SyntaxTree_Base* pOP3)
        : parent(sizeof(SyntaxTree_Exp))
        , OP1(OP1)
        , pOP2(pOP2)
        , pOP3(pOP3)
        , _type(TrueFalse)
    {
    }
    
    SyntaxTree_Exp::SyntaxTree_Exp(Type type, const SyntaxTree_Base& OP1, SyntaxTree_Base* pOP2)
        : parent(sizeof(SyntaxTree_Exp))
        , OP1(OP1)
        , pOP2(pOP2)
        , pOP3(NULL)
        , _type(type)
    {
    }

    SyntaxTree_Exp::SyntaxTree_Exp(Type type, const SyntaxTree_Base& OP)
        : parent(sizeof(SyntaxTree_Exp))
        , OP1(OP)
        , pOP2(NULL)
        , pOP3(NULL)
        , _type(type)
    {
    }

    SyntaxTree_Exp::~SyntaxTree_Exp()
    {
    }

    void SyntaxTree_Exp::print(ostream& stream, uint indent)const
    {
        switch (_type)
        {
        case Not:
            stream << "!";
            break;
        case Positive:
            stream << "+";
            break;
        case Negative:
            stream << "-";
            break;
        default:
            break;
        }
        OP1.print(stream, indent);
        switch (_type)
        {
        case GreaterEqual:
            stream << " >= ";
            break;
        case LessEqual:
            stream << " <= ";
            break;
        case Equal:
            stream << " == ";
            break;
        case Greater:
            stream << " > ";
            break;
        case Less:
            stream << " < ";
            break;
        case Assign:
            stream << " = ";
            break;
        case LogicAnd:
            stream << " && ";
            break;
        case LogicOr:
            stream << " || ";
            break;
        case BitAnd:
            stream << " & ";
            break;
        case BitOr:
            stream << " | ";
            break;
        case BitXor:
            stream << " ^ ";
            break;
        case TrueFalse:
            stream << " ? ";
            break;
        case Add:
            stream << " + ";
            break;
        case Sub:
            stream << " - ";
            break;
        case Mul:
            stream << " * ";
            break;
        case Div:
            stream << " / ";
            break;
        case Mod:
            stream << " % ";
            break;
        case ValueAsType:
            stream << " as ";
            break;
        default:
            break;
        }
        if (pOP2) pOP2->print(stream, indent);
        if (pOP3)
        {
            stream << " : ";
            pOP3->print(stream, indent);
        }
    }
    
    // exp -> exp "?" exp ":" exp
    bool Parser::reduceExp3Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        
        SyntaxTree_Exp* pExp = allocator<SyntaxTree_Exp>::allocate();
        construct(pExp, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[2]), syntaxTreeStack[1], syntaxTreeStack.top());
        
        context.data.insert(pExp);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pExp);
        
        return true;
    }

    // exp -> exp ">" "=" exp1
    // exp -> exp "<" "=" exp1
    // exp -> exp "=" "=" exp1
    // exp -> exp "&" "&" exp1
    // exp -> exp "|" "|" exp1
    bool Parser::reduceExp2Size(ushort i)
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Exp* pExp = allocator<SyntaxTree_Exp>::allocate();
        switch (i)
        {
        case EXP_GREATER_EQUAL_EXP1:
            construct(pExp, SyntaxTree_Exp::GreaterEqual, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_LESS_EQUAL:
            construct(pExp, SyntaxTree_Exp::LessEqual, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_EQUAL:
            construct(pExp, SyntaxTree_Exp::Equal, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_LOGIC_AND:
            construct(pExp, SyntaxTree_Exp::LogicAnd, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_LOGIC_OR:
            construct(pExp, SyntaxTree_Exp::LogicOr, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        }

        context.data.insert(pExp);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pExp);

        return true;
    }

    // exp -> exp ">" exp1
    // exp -> exp "<" exp1
    // exp -> exp "=" exp1
    // exp -> exp "&" exp1
    // exp -> exp "|" exp1
    // exp -> exp "^" exp1
    // exp1 -> "!" exp2
    // exp1 -> "+" exp2
    // exp1 -> "-" exp2
    // exp2 -> exp2 "+" exp3
    // exp2 -> exp2 "-" exp3
    // exp3 -> exp3 "*" exp4
    // exp3 -> exp3 "/" exp4
    // exp3 -> exp3 "%" exp4
    bool Parser::reduceExp1Size(ushort i)
    {
#ifdef _DEBUG
        switch (i)
        {
        case EXP_GREATER:
        case EXP_LESS:
        case EXP_ASSIGN:
        case EXP_BIT_AND:
        case EXP_BIT_OR:
        case EXP_BIT_XOR:
        case EXP2_ADD:
        case EXP2_SUB:
        case EXP3_MUL:
        case EXP3_DIV:
        case EXP3_MOD:
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
            break;
        case EXP_NOT:
        case EXP1_POSITIVE:
        case EXP1_NEGATIVE:
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
            break;
        }
#endif
        shifts.pop();

        SyntaxTree_Exp* pExp = allocator<SyntaxTree_Exp>::allocate();
        switch (i)
        {
        case EXP_GREATER:
            construct(pExp, SyntaxTree_Exp::Greater, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_LESS:
            construct(pExp, SyntaxTree_Exp::Less, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_ASSIGN:
            construct(pExp, SyntaxTree_Exp::Assign, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_BIT_AND:
            construct(pExp, SyntaxTree_Exp::BitAnd, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_BIT_OR:
            construct(pExp, SyntaxTree_Exp::BitOr, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_BIT_XOR:
            construct(pExp, SyntaxTree_Exp::BitXor, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP_NOT:
            construct(pExp, SyntaxTree_Exp::Not, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack.top()));
            break;
        case EXP1_POSITIVE:
            construct(pExp, SyntaxTree_Exp::Positive, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack.top()));
            break;
        case EXP1_NEGATIVE:
            construct(pExp, SyntaxTree_Exp::Negative, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack.top()));
            break;
        case EXP2_ADD:
            construct(pExp, SyntaxTree_Exp::Add, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP2_SUB:
            construct(pExp, SyntaxTree_Exp::Sub, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP3_MUL:
            construct(pExp, SyntaxTree_Exp::Mul, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP3_DIV:
            construct(pExp, SyntaxTree_Exp::Div, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        case EXP3_MOD:
            construct(pExp, SyntaxTree_Exp::Mod, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());
            break;
        }

        context.data.insert(pExp);

        syntaxTreeStack.pop();
        switch (i)
        {
        case EXP_GREATER:
        case EXP_LESS:
        case EXP_ASSIGN:
        case EXP_BIT_AND:
        case EXP_BIT_OR:
        case EXP_BIT_XOR:
        case EXP2_ADD:
        case EXP2_SUB:
        case EXP3_MUL:
        case EXP3_DIV:
        case EXP3_MOD:
            syntaxTreeStack.pop();
            break;
        }
        syntaxTreeStack.push(pExp);

        return true;
    }

    // exp4 -> call_desc
    bool Parser::reduceExpCall()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Call*, syntaxTreeStack.top());
#endif
        SyntaxTree_Exp* pExp = allocator<SyntaxTree_Exp>::allocate();
        construct(pExp, SyntaxTree_Exp::Call, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack.top()));

        context.data.insert(pExp);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pExp);

        return true;
    }

    // exp4 -> value_desc "as" type_desc
    bool Parser::reduceExpValueAsType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Value*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack.top());
#endif
        shifts.pop();

        SyntaxTree_Exp* pExp = allocator<SyntaxTree_Exp>::allocate();
        construct(pExp, SyntaxTree_Exp::ValueAsType, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack[1]), syntaxTreeStack.top());

        context.data.insert(pExp);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pExp);

        return true;
    }

    // exp4 -> value_desc
    bool Parser::reduceExpValue()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Value*, syntaxTreeStack.top());
#endif
        SyntaxTree_Exp* pExp = allocator<SyntaxTree_Exp>::allocate();
        construct(pExp, SyntaxTree_Exp::Value, dynamic_cast<const SyntaxTree_Base&>(*syntaxTreeStack.top()));

        context.data.insert(pExp);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pExp);

        return true;
    }
}
