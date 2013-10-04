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
            stream << "!(";
            OP1.print(stream, indent);
            stream << ')';
            return;
        case Positive:
            stream << "+(";
            OP1.print(stream, indent);
            stream << ')';
            return;
        case Negative:
            stream << "-(";
            OP1.print(stream, indent);
            stream << ')';
            return;
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
        switch (_type)
        {
        case Value:
            {
                if (dynamic_cast<const SyntaxTree_Value&>(OP1).isConstValue())
                {
                    VM::Variant v = dynamic_cast<const SyntaxTree_Value&>(OP1).toVariant();
                    const pair<int, ushort> p = pParser->indexOfConstant(v);
                    if (p.first == -1)
                    {
                        const pair<short, ushort> pa = pParser->pushConstant(v);
                        ucBlock = (uchar)pa.first;
                        usIndex = pa.second;
                    }
                    else
                    {
                        ucBlock = p.first;
                        usIndex = p.second;
                    }
                    bConstant = true;
                }
                else
                {
                    pair<short, ushort> p = pParser->indexOfRegister(dynamic_cast<const SyntaxTree_Value&>(OP1).name());
                    if (p.first == -1)
                    {
                        string str = string::format("undefined variable %s", dynamic_cast<const SyntaxTree_Value&>(OP1).value.c_str());
                        throw error<const char*>(str.c_str(), __FILE__, __LINE__);
                    }
                    ucBlock = (uchar)p.first;
                    usIndex = p.second;
                }
            }
            break;
        case TrueFalse:
            {
                if (!const_cast<SyntaxTree_Base&>(OP1).make(pParser)) return false;
                if (OP1.isConstValue())
                {
                    if (dynamic_cast<const SyntaxTree_Exp&>(OP1).toVariant(pParser).toBool())
                    {
                        if (!pOP2->make(pParser)) return false;
                        if (pOP2->isConstValue()) // 可提前计算出结果
                        {
                            ucBlock = dynamic_cast<const SyntaxTree_Exp*>(pOP2)->ucBlock;
                            usIndex = dynamic_cast<const SyntaxTree_Exp*>(pOP2)->usIndex;
                            bConstant = true;
                        }
                    }
                    else
                    {
                        if (!pOP3->make(pParser)) return false;
                        if (pOP3->isConstValue()) // 可提前计算出结果
                        {
                            ucBlock = dynamic_cast<const SyntaxTree_Exp*>(pOP3)->ucBlock;
                            usIndex = dynamic_cast<const SyntaxTree_Exp*>(pOP3)->usIndex;
                            bConstant = true;
                        }
                    }
                }
                else
                {
                    // | op1 code              |
                    // | jmp lb1 when false    |
                    // | op2 code              |
                    // | end mov to register p |
                    // | jmp lb2               |
                    // | lb1:                  |
                    // | op3 code              |
                    // | end mov to register p |
                    // | lb2:                  |
                    // | ...                   |
                    pair<short, ushort> p = pParser->tmpRegister();
                    if (p.first == -1)
                    {
                        throw error<const char*>("have no register", __FILE__, __LINE__);
                        return false;
                    }
                    VM::Instruction i;
                    i.op = VM::OpCode::Jmp;
                    i.ot = MAKE_OT(0, 0, 0);
                    i.Jmp.ob  = dynamic_cast<const SyntaxTree_Exp&>(OP1).ucBlock;
                    i.Jmp.os  = dynamic_cast<const SyntaxTree_Exp&>(OP1).usIndex;
                    i.Jmp.ext = true;
                    pParser->instructions.push_back(i); // 判断表达式是否为false并跳转
                    size_t ri = pParser->instructions.size() - 1; // 地址需回填
                    size_t addr = pParser->instructions.size();
                    if (!pOP2->make(pParser)) return false;
                    if (pOP2->isConstValue())
                    {
                        if (!make_constant(pParser, *dynamic_cast<const SyntaxTree_Exp*>(pOP2), p)) return false;
                    }
                    else
                    {
                        if (!make_register(pParser, *dynamic_cast<const SyntaxTree_Exp*>(pOP2), p)) return false;
                    }
                    if (!insert_flag_register(pParser, p)) return false; // mov本身并不设置flag寄存器，应此需要手动设置
                    addr = pParser->instructions.size() - addr;
                    pParser->instructions[ri].Jmp.addr = addr;

                    i.op = VM::OpCode::Jmp;
                    i.Jmp.ext = false;
                    pParser->instructions.push_back(i);
                    ri = pParser->instructions.size() - 1;
                    addr = pParser->instructions.size();
                    if (!pOP3->make(pParser)) return false;
                    if (pOP3->isConstValue())
                    {
                        if (!make_constant(pParser, *dynamic_cast<const SyntaxTree_Exp*>(pOP3), p)) return false;
                    }
                    else
                    {
                        if (!make_register(pParser, *dynamic_cast<const SyntaxTree_Exp*>(pOP3), p)) return false;
                    }
                    if (!insert_flag_register(pParser, p)) return false; // mov本身并不设置flag寄存器，应此需要手动设置
                    addr = pParser->instructions.size() - addr;
                    pParser->instructions[ri].Jmp.addr = addr;
                    ucBlock = (uchar)p.first;
                    usIndex = p.second;
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
                    VM::Variant v = eval(dynamic_cast<const SyntaxTree_Exp&>(OP1).toVariant(pParser), _type);
                    const pair<int, ushort> p = pParser->indexOfConstant(v);
                    if (p.first == -1)
                    {
                        const pair<short, ushort> pa = pParser->pushConstant(v);
                        ucBlock = (uchar)pa.first;
                        usIndex = pa.second;
                    }
                    else
                    {
                        ucBlock = p.first;
                        usIndex = p.second;
                    }
                    bConstant = true;
                }
                else return make_op1(pParser, pair<uchar, ushort>(dynamic_cast<const SyntaxTree_Exp&>(OP1).ucBlock, dynamic_cast<const SyntaxTree_Exp&>(OP1).usIndex), _type);
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
                        const pair<short, ushort> pa = pParser->pushConstant(v);
                        ucBlock = (uchar)pa.first;
                        usIndex = pa.second;
                    }
                    else
                    {
                        ucBlock = p.first;
                        usIndex = p.second;
                    }
                    bConstant = true;
                }
                else
                {
                    if (bConstValue1)
                        return make_op2(pParser,
                                        pair<uchar, ushort>(dynamic_cast<const SyntaxTree_Exp&>(OP1).ucBlock, dynamic_cast<const SyntaxTree_Exp&>(OP1).usIndex),
                                        *dynamic_cast<const SyntaxTree_Exp*>(pOP2),
                                        _type);
                    else if (bConstValue2)
                        return make_op2(pParser,
                                        dynamic_cast<const SyntaxTree_Exp&>(OP1),
                                        pair<uchar, ushort>(dynamic_cast<const SyntaxTree_Exp*>(pOP2)->ucBlock, dynamic_cast<const SyntaxTree_Exp*>(pOP2)->usIndex),
                                        _type);
                    else return make_op2(pParser, dynamic_cast<const SyntaxTree_Exp&>(OP1), *dynamic_cast<const SyntaxTree_Exp*>(pOP2), _type);
                }
            }
            break;
        default:
            break;
        }
        return true;
    }

    bool SyntaxTree_Exp::make_register(Parser* pParser, const SyntaxTree_Exp& op, const pair<uchar, ushort>& reg)
    {
        VM::Instruction i;
        i.op = VM::OpCode::Mov;
        i.ot = MAKE_OT(0, 0, 0);
        i.Normal.ob1 = op.ucBlock;
        i.Normal.os1 = op.usIndex;
        i.Normal.obd = reg.first;
        i.Normal.od  = reg.second;
        pParser->instructions.push_back(i);
        return true;
    }

    bool SyntaxTree_Exp::make_constant(Parser* pParser, const SyntaxTree_Exp& op, const pair<uchar, ushort>& reg)
    {
        VM::Instruction i;
        i.op = VM::OpCode::Mov;
        i.ot = MAKE_OT(1, 0, 0);
        i.Normal.ob1 = op.ucBlock;
        i.Normal.os1 = op.usIndex;
        i.Normal.obd = reg.first;
        i.Normal.od  = reg.second;
        pParser->instructions.push_back(i);
        return true;
    }

    bool SyntaxTree_Exp::insert_flag_register(Parser* pParser, const pair<uchar, ushort>& reg)
    {
        VM::Instruction i;
        i.op = VM::OpCode::Mov;
        i.ot = MAKE_OT(0, 0, 0);
        i.Normal.ob1 = reg.first;
        i.Normal.os1 = reg.second;
        i.Normal.obd = 0;
        i.Normal.od  = 65534;
        pParser->instructions.push_back(i);
        return true;
    }

    bool SyntaxTree_Exp::make_op1(Parser* pParser, const pair<uchar, ushort>& op, Type type)
    {
        VM::Instruction i;
        i.ot = MAKE_OT(0, 0, 0);
        i.Normal.ob1 = op.first;
        i.Normal.os1 = op.second;
        switch (type)
        {
        case Not:
            i.op = VM::OpCode::Not;
            break;
        case Positive:
            i.op = VM::OpCode::Pos;
            break;
        case Negative:
            i.op = VM::OpCode::Neg;
            break;
        default:
            throw error<const char*>("can't do here", __FILE__, __LINE__);
            return false;
        }
        pParser->instructions.push_back(i);
        ucBlock = op.first;
        usIndex = op.second;
        return true;
    }

    void SyntaxTree_Exp::fill_op2(Type type, VM::Instruction& i)
    {
        switch (type)
        {
        case GreaterEqual:
            i.op = VM::OpCode::MoreEqual;
            break;
        case LessEqual:
            i.op = VM::OpCode::LessEqual;
            break;
        case Equal:
            i.op = VM::OpCode::Equal;
            break;
        case Greater:
            i.op = VM::OpCode::More;
            break;
        case Less:
            i.op = VM::OpCode::Less;
            break;
        case LogicAnd:
            i.op = VM::OpCode::LogicAnd;
            break;
        case LogicOr:
            i.op = VM::OpCode::LogicOr;
            break;
        case BitAnd:
            i.op = VM::OpCode::BitAnd;
            break;
        case BitOr:
            i.op = VM::OpCode::BitOr;
            break;
        case BitXor:
            i.op = VM::OpCode::BitXor;
            break;
        case Add:
            i.op = VM::OpCode::Add;
            break;
        case Sub:
            i.op = VM::OpCode::Sub;
            break;
        case Mul:
            i.op = VM::OpCode::Mul;
            break;
        case Div:
            i.op = VM::OpCode::Div;
            break;
        case Mod:
            i.op = VM::OpCode::Mod;
            break;
        default:
            throw error<const char*>("can't do here", __FILE__, __LINE__);
            break;
        }
    }

    bool SyntaxTree_Exp::make_op2(Parser* pParser, const pair<uchar, ushort> &op1, const SyntaxTree_Exp &op2, Type type)
    {
        pair<short, ushort> p = pParser->tmpRegister();
        if (p.first == -1)
        {
            throw error<const char*>("have no register", __FILE__, __LINE__);
            return false;
        }
        VM::Instruction i;
        i.ot = MAKE_OT(1, 0, 0);
        i.Normal.ob1 = op1.first;
        i.Normal.os1 = op1.second;
        i.Normal.ob2 = op2.ucBlock;
        i.Normal.os2 = op2.usIndex;
        i.Normal.obd = (uchar)p.first;
        i.Normal.od  = p.second;
        fill_op2(type, i);
        pParser->instructions.push_back(i);
        ucBlock = (uchar)p.first;
        usIndex = p.second;
        return true;
    }

    bool SyntaxTree_Exp::make_op2(Parser* pParser, const SyntaxTree_Exp &op1, const pair<uchar, ushort> &op2, Type type)
    {
        pair<short, ushort> p = pParser->tmpRegister();
        if (p.first == -1)
        {
            throw error<const char*>("have no register", __FILE__, __LINE__);
            return false;
        }
        VM::Instruction i;
        i.ot = MAKE_OT(0, 1, 0);
        i.Normal.ob1 = op1.ucBlock;
        i.Normal.os1 = op1.usIndex;
        i.Normal.ob2 = op2.first;
        i.Normal.os2 = op2.second;
        i.Normal.obd = (uchar)p.first;
        i.Normal.od  = p.second;
        fill_op2(type, i);
        pParser->instructions.push_back(i);
        ucBlock = (uchar)p.first;
        usIndex = p.second;
        return true;
    }

    bool SyntaxTree_Exp::make_op2(Parser* pParser, const SyntaxTree_Exp &op1, const SyntaxTree_Exp &op2, Type type)
    {
        pair<short, ushort> p = pParser->tmpRegister();
        if (p.first == -1)
        {
            throw error<const char*>("have no register", __FILE__, __LINE__);
            return false;
        }
        VM::Instruction i;
        i.ot = MAKE_OT(0, 0, 0);
        i.Normal.ob1 = op1.ucBlock;
        i.Normal.os1 = op1.usIndex;
        i.Normal.ob2 = op2.ucBlock;
        i.Normal.os2 = op2.usIndex;
        i.Normal.obd = (uchar)p.first;
        i.Normal.od  = p.second;
        fill_op2(type, i);
        pParser->instructions.push_back(i);
        ucBlock = (uchar)p.first;
        usIndex = p.second;
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
