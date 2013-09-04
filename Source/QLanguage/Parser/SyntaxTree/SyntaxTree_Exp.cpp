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
#include "SyntaxTree_CallList.h"
#include "SyntaxTree_Value.h"
#include "SyntaxTree_Type.h"
#include "SyntaxTree_Exp.h"

namespace QLanguage
{
    SyntaxTree_Exp::SyntaxTree_Exp(const SyntaxTree_Base& OP1, SyntaxTree_Base* pOP2, SyntaxTree_Base* pOP3)
        : parent(sizeof(SyntaxTree_Exp))
        , OP1(OP1)
        , pOP2(pOP2)
        , pOP3(pOP3)
        , _type(TrueFalse)
        , bConstant(false)
    {
    }
    
    SyntaxTree_Exp::SyntaxTree_Exp(Type type, const SyntaxTree_Base& OP1, SyntaxTree_Base* pOP2)
        : parent(sizeof(SyntaxTree_Exp))
        , OP1(OP1)
        , pOP2(pOP2)
        , pOP3(NULL)
        , _type(type)
        , bConstant(false)
    {
    }

    SyntaxTree_Exp::SyntaxTree_Exp(Type type, const SyntaxTree_Base& OP)
        : parent(sizeof(SyntaxTree_Exp))
        , OP1(OP)
        , pOP2(NULL)
        , pOP3(NULL)
        , _type(type)
        , bConstant(false)
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

    const HASH_KEY_TYPE SyntaxTree_Exp::hash()const
    {
        if (_type == Value) return OP1.hash();
        return 0;
    }

    const bool SyntaxTree_Exp::toBool()const
    {
        if (_type == Value) return OP1.toBool();
        return false;
    }

    bool SyntaxTree_Exp::make(Parser* pParser)
    {
        // TODO
        switch (_type)
        {
        case Value:
            {
                VM::Variant v = dynamic_cast<const SyntaxTree_Value&>(OP1).toVariant();
                const pair<int, ushort> p = pParser->indexOfConstant(v);
                if (p.first == -1)
                {
                    const pair<size_t, ushort> pa = pParser->pushConstant(v);
                    constantBlock = pa.first;
                    constantIndex = pa.second;
                }
                else
                {
                    constantBlock = p.first;
                    constantIndex = p.second;
                }
                bConstant = true;
            }
            break;
        case TrueFalse:
            {
                if (!const_cast<SyntaxTree_Base&>(OP1).make(pParser)) return false;
                if (!pOP2->make(pParser)) return false;
                if (!pOP3->make(pParser)) return false;
                if (OP1.isValue() && OP1.isConstValue())
                {
                    if (dynamic_cast<const SyntaxTree_Value&>(dynamic_cast<const SyntaxTree_Exp&>(OP1).OP1).toBool())
                    {
                        if (pOP2->isValue() && pOP2->isConstValue()) // 可提前计算出结果
                        {
                            VM::Variant v = eval(dynamic_cast<const SyntaxTree_Exp*>(pOP2)->toVariant(pParser), dynamic_cast<const SyntaxTree_Exp*>(pOP2)->_type);
                            const pair<int, ushort> p = pParser->indexOfConstant(v);
                            if (p.first == -1)
                            {
                                const pair<size_t, ushort> pa = pParser->pushConstant(v);
                                constantBlock = pa.first;
                                constantIndex = pa.second;
                            }
                            else
                            {
                                constantBlock = p.first;
                                constantIndex = p.second;
                            }
                            bConstant = true;
                        }
                    }
                    else
                    {
                        if (pOP3->isValue() && pOP3->isConstValue()) // 可提前计算出结果
                        {
                            VM::Variant v = eval(dynamic_cast<const SyntaxTree_Exp*>(pOP3)->toVariant(pParser), dynamic_cast<const SyntaxTree_Exp*>(pOP3)->_type);
                            const pair<int, ushort> p = pParser->indexOfConstant(v);
                            if (p.first == -1)
                            {
                                const pair<size_t, ushort> pa = pParser->pushConstant(v);
                                constantBlock = pa.first;
                                constantIndex = pa.second;
                            }
                            else
                            {
                                constantBlock = p.first;
                                constantIndex = p.second;
                            }
                            bConstant = true;
                        }
                    }
                }
            }
            break;
        case Not:
        case Positive:
        case Negative:
            {
                if (!const_cast<SyntaxTree_Base&>(OP1).make(pParser)) return false;
                if (OP1.isConstValue())
                {
                    VM::Variant v = eval(dynamic_cast<const SyntaxTree_Exp&>(OP1).toVariant(pParser), dynamic_cast<const SyntaxTree_Exp&>(OP1)._type);
                    const pair<int, ushort> p = pParser->indexOfConstant(v);
                    if (p.first == -1)
                    {
                        const pair<size_t, ushort> pa = pParser->pushConstant(v);
                        constantBlock = pa.first;
                        constantIndex = pa.second;
                    }
                    else
                    {
                        constantBlock = p.first;
                        constantIndex = p.second;
                    }
                    bConstant = true;
                }
            }
            break;
        case GreaterEqual:
        case LessEqual:
        case Equal:
        case Greater:
        case Less:
        case LogicAnd:
        case LogicOr:
        case BitAnd:
        case BitOr:
        case BitXor:
        case Add:
        case Sub:
        case Mul:
        case Div:
        case Mod:
            {
                if (!const_cast<SyntaxTree_Base&>(OP1).make(pParser)) return false;
                if (!pOP2->make(pParser)) return false;

                bool bConstValue1 = OP1.isConstValue();
                bool bConstValue2 = pOP2->isConstValue();

                if (bConstValue1 && bConstValue2) // 两个常量可提前计算出结果
                {
                    VM::Variant v = eval(dynamic_cast<const SyntaxTree_Exp&>(OP1).toVariant(pParser),
                                         dynamic_cast<const SyntaxTree_Exp*>(pOP2)->toVariant(pParser),
                                         _type);
                    const pair<int, ushort> p = pParser->indexOfConstant(v);
                    if (p.first == -1)
                    {
                        const pair<size_t, ushort> pa = pParser->pushConstant(v);
                        constantBlock = pa.first;
                        constantIndex = pa.second;
                    }
                    else
                    {
                        constantBlock = p.first;
                        constantIndex = p.second;
                    }
                    bConstant = true;
                }
            }
            break;
        }
        return true;
    }

    const VM::Variant SyntaxTree_Exp::eval(const VM::Variant& v1, const VM::Variant& v2, Type type)
    {
        switch (type)
        {
        case GreaterEqual:
            return v1 >= v2;
        case LessEqual:
            return v1 <= v2;
        case Equal:
            return v1 == v2;
        case Greater:
            return v1 > v2;
        case Less:
            return v1 < v2;
        case LogicAnd:
            return v1 && v2;
        case LogicOr:
            return v1 || v2;
        case BitAnd:
            return v1 & v2;
        case BitOr:
            return v1 | v2;
        case BitXor:
            return v1 ^ v2;
        case Add:
            return v1 + v2;
        case Sub:
            return v1 - v2;
        case Mul:
            return v1 * v2;
        case Div:
            return v1 / v2;
        case Mod:
            return v1 % v2;
        case Assign:
        case Not:
        case Positive:
        case Negative:
        case TrueFalse:
        case Call:
        case ValueAsType:
        case Value:
            throw error<const char*>("Can't eval with operator", __FILE__, __LINE__); // 两个常量不可能有这些运算符
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    const VM::Variant SyntaxTree_Exp::eval(const VM::Variant& v, Type type)
    {
        switch (type)
        {
        case Not:
            return !v;
        case Positive:
            return +v;
        case Negative:
            return -v;
        case Value:
            return v;
        default:
            throw error<const char*>("Can't eval with operator", __FILE__, __LINE__);
            return VM::Variant();
        }

        return VM::Variant();
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

    // exp4 -> call_list
    bool Parser::reduceExpCall()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_CallList*, syntaxTreeStack.top());
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
