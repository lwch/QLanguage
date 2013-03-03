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
        Lexer();
        ~Lexer();

        bool parse(const string& input);
    public:
        list<string> result;
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