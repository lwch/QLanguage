/********************************************************************
	created:	2013/05/10
	created:	10:5:2013   12:58
	filename: 	\Calculator\Parser\Parser.cpp
	file path:	\Calculator\Parser
	file base:	Parser
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <math.h>

#include "Parser.h"

namespace QLanguage
{
    Parser::Parser(const vector<Production>& productions) : BasicParser(productions)
    {
    }

    Parser::~Parser()
    {
    }

    bool Parser::shift(const string& s)
    {
        shifts.push_back(s);
        return true;
    }

    bool Parser::reduce(ushort i)
    {
        switch (i)
        {
        case 0:
            cout << "Result is: " << numbers.top();
            return true;
        case 1: // value_type -> "{Real}"
            return reduceReal();
        case 2: // value_type -> "{Digit}"
            return reduceDigit();
        case 3: // exp -> exp "+" term
            return reduceAdd();
        case 4: // exp -> exp "-" term
            return reduceSub();
        case 5:
            break;
        case 6: // term -> term "*" factor
            return reduceMul();
        case 7: // term -> term "/" factor
            return reduceDiv();
        case 8:
            break;
        case 9: // factor -> "(" exp ")"
            return reduceBrackets();
        case 10: // factor -> "sin" "(" exp ")"
            return reduceSin();
        case 11: // factor -> "cos" "(" exp ")"
            return reduceCos();
        case 12: // factor -> "log" "(" exp ")"
            return reduceLog();
        case 13: // exp -> "+" value_type
        case 14: // exp -> "-" value_type
            return reduceSymValue();
        case 15: // exp -> value_type
            break;
        default:
            return false;
        }
        return true;
    }

    // value_type -> "{Real}"
    bool Parser::reduceReal()
    {
        numbers.push(atof(shifts.back().c_str()));
        shifts.pop_back();
        return true;
    }

    // value_type -> "{Digit}"
    bool Parser::reduceDigit()
    {
        numbers.push(atoi(shifts.back().c_str()));
        shifts.pop_back();
        return true;
    }

    // exp -> exp "+" term
    bool Parser::reduceAdd()
    {
        double op2 = numbers.top();
        numbers.pop();
        double op1 = numbers.top();
        numbers.pop();
        numbers.push(op1 + op2);
        return true;
    }

    // exp -> exp "-" term
    bool Parser::reduceSub()
    {
        double op2 = numbers.top();
        numbers.pop();
        double op1 = numbers.top();
        numbers.pop();
        numbers.push(op1 - op2);
        return true;
    }

    // term -> term "*" factor
    bool Parser::reduceMul()
    {
        double op2 = numbers.top();
        numbers.pop();
        double op1 = numbers.top();
        numbers.pop();
        numbers.push(op1 * op2);
        return true;
    }

    // term -> term "/" factor
    bool Parser::reduceDiv()
    {
        double op2 = numbers.top();
        numbers.pop();
        double op1 = numbers.top();
        numbers.pop();
        numbers.push(op1 / op2);
        return true;
    }

    // factor -> "(" exp ")"
    bool Parser::reduceBrackets()
    {
        shifts.pop_back();
        shifts.pop_back();
        return true;
    }

    // factor -> "sin" "(" exp ")"
    bool Parser::reduceSin()
    {
        shifts.pop_back();
        shifts.pop_back();
        shifts.pop_back();
        double d = sin(numbers.top());
        numbers.pop();
        numbers.push(d);
        return true;
    }

    // factor -> "cos" "(" exp ")"
    bool Parser::reduceCos()
    {
        shifts.pop_back();
        shifts.pop_back();
        shifts.pop_back();
        double d = cos(numbers.top());
        numbers.pop();
        numbers.push(d);
        return true;
    }

    // factor -> "log" "(" exp ")"
    bool Parser::reduceLog()
    {
        shifts.pop_back();
        shifts.pop_back();
        shifts.pop_back();
        double d = log(numbers.top());
        numbers.pop();
        numbers.push(d);
        return true;
    }

    // exp -> "+" value_type
    // exp -> "-" value_type
    bool Parser::reduceSymValue()
    {
        if (shifts.back() == "-") numbers.top() = -numbers.top();
        shifts.pop_back();
        return true;
    }
}