/********************************************************************
	created:	2013/03/03
	created:	3:3:2013   16:42
	filename: 	\QLanguage\Lexer\Lexer.h
	file path:	\QLanguage\Lexer
	file base:	Lexer
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LEXER_H_
#define _QLANGUAGE_LEXER_H_

#include "../../QCore/Library/list.h"
#include "../../QCore/Library/string.h"
#include "../../QCore/Library/regex/regex.h"

namespace QLanguage
{
    using namespace QLanguage::Library;
    using namespace QLanguage::Library::regex;

    class Lexer
    {
    public:
        struct Token
        {
            enum Type
            {
                Digit,
                Hex,
                Real,
                Letter,
                String,
                Operator,
                Space,
                Enter,
            }type;

            string data;

            Token() {}
            Token(Type type, const string& data) : type(type), data(data) {}
        };
    public:
        Lexer();
        ~Lexer();

        bool parse(const string& input);
    public:
        list<Token> result;
    protected:
        Rule::Context context;
        // Number Begin
        Rule _0;
        Rule _9;
        Rule _0x;
        Rule _0X;
        Rule _point;
        Rule digit;
        Rule hex;
        Rule real;
        // Number End

        // Letter Begin
        Rule _;
        Rule _a;
        Rule _z;
        Rule _A;
        Rule _Z;
        Rule letter;
        // Letter End

        // String Begin
        Rule quotationMarks;
        Rule str;
        // String End

        // Operator Begin
        Rule optr;
        // Operator End

        // Space Begin
        Rule _space;
        Rule _tab;
        Rule space;
        // Space End

        // Tools Start
        Rule _0_9;
        Rule _a_z;
        Rule _A_Z;
        // Tools End

        // Enter Start
        Rule _r;
        Rule _n;
        Rule _rn;
        Rule _nr;
        Rule enter;
        // Enter End
    };
}

#endif
