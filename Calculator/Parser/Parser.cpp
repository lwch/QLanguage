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
        case 3: // exp -> "+" value_type
        case 4: // exp -> "-" value_type
            return reduceSymValue();
        case 5:
            break;
        case 6: // exp -> exp "+" term
            return reduceAdd();
        case 7: // exp -> exp "-" term
            return reduceSub();
        case 8:
            break;
        case 9: // term -> term "*" factor
            return reduceMul();
        case 10: // term -> term "/" factor
            return reduceDiv();
        case 11:
            break;
        case 12: // factor -> "sin" bracket
            return reduceSin();
        case 13: // factor -> "cos" bracket
            return reduceCos();
        case 14: // factor -> "log" bracket
            return reduceLog();
        case 15:
            break;
        case 16:
            break;
        case 17: // bracket -> "(" sym ")"
            return reduceBrackets();
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
        shifts.pop_back();
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
        shifts.pop_back();
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
        shifts.pop_back();
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
        shifts.pop_back();
        return true;
    }

    // factor -> "(" exp ")"
    bool Parser::reduceBrackets()
    {
        shifts.pop_back();
        shifts.pop_back();
        return true;
    }

    // factor -> "sin" bracket
    bool Parser::reduceSin()
    {
        shifts.pop_back();
        double d = sin(numbers.top());
        numbers.pop();
        numbers.push(d);
        return true;
    }

    // factor -> "cos" bracket
    bool Parser::reduceCos()
    {
        shifts.pop_back();
        double d = cos(numbers.top());
        numbers.pop();
        numbers.push(d);
        return true;
    }

    // factor -> "log" bracket
    bool Parser::reduceLog()
    {
        shifts.pop_back();
        double d = log(numbers.top());
        numbers.pop();
        numbers.push(d);
        return true;
    }

    // sym -> "+" exp
    // sym -> "-" exp
    bool Parser::reduceSymValue()
    {
        if (shifts.back() == "-") numbers.top() = -numbers.top();
        shifts.pop_back();
        return true;
    }
}