/********************************************************************
	created:	2013/03/03
	created:	3:3:2013   16:42
	filename: 	\QLanguage\Lexer\Lexer.cpp
	file path:	\QLanguage\Lexer
	file base:	Lexer
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "Lexer.h"

namespace QLanguage
{
    Lexer::Lexer()
    {
        // Number Begin
        _0     = Rule('0', &context);
        _9     = Rule('9', &context);
        _0x    = Rule("0x", &context);
        _0X    = Rule("0X", &context);
        _point = Rule('.', &context);
        _0_9   = _0 - _9;
        digit  = +_0_9;
        hex    = (_0x | _0X) + digit;
        real   = digit.opt() + _point + digit;
        digit.buildDFA();
        hex.buildDFA();
        real.buildDFA();
        // Number End

        // Letter Begin
        _      = Rule('_', &context);
        _a     = Rule('a', &context);
        _z     = Rule('z', &context);
        _A     = Rule('A', &context);
        _Z     = Rule('Z', &context);
        _a_z   = _a - _z;
        _A_Z   = _A - _Z;
        letter = ((+_ + digit) |
                  (+(_ | _a_z | _A_Z))) +
                  *(_ | digit | _a_z | _A_Z);
        letter.buildDFA();
        // Letter End

        // Space Start
        _space = Rule(' ', &context);
        _tab   = Rule('\t', &context);
        space  = +(_space | _tab);
        space.buildDFA();
        // Space End

        // Enter Start
        _r    = Rule('\r', &context);
        _n    = Rule('\n', &context);
        _rn   = Rule("\r\n", &context);
        _nr   = Rule("\n\r", &context);
        enter = +(_r | _n | _rn | _nr);
        enter.buildDFA();
        // Enter End
    }

    Lexer::~Lexer()
    {
    }

    bool Lexer::parse(const string& input)
    {
        return true;
    }
}
