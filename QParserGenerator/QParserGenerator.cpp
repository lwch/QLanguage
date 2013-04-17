/********************************************************************
	created:	2013/04/11
	created:	11:4:2013   13:12
	filename: 	\QParserGenerator\QParserGenerator.cpp
	file path:	\QParserGenerator
	file base:	QParserGenerator
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../QCore/Library/Console.h"
#include "../QCore/Library/fstream.h"

#include "../QLanguage/Lexer/Lexer.h"
#include "../QLanguage/Parser/LR0.h"
#include "../QLanguage/Parser/LALR1.h"

#include <stdio.h>
#include <time.h>

using namespace QLanguage;
using namespace QLanguage::Library;

int main(int argv, char** args)
{
    if (argv >= 2)
    {
        try
        {
            // step1 lexer
            fstream fs(args[1], fstream::in | fstream::binary);
            string str(fs.size());

            fs.readAll(const_cast<char*>(str.c_str()), str.capacity());
            const_cast<char*>(str.c_str())[fs.size()] = 0;
            str.setsize(fs.size());
            fs.close();

            Lexer lexer;
            clock_t c = clock();
            lexer.parse(str);
            c = clock() - c;

            Console::SetColor(false, true, false, true);
            Console::WriteLine("Lexer Finish ...");
            Console::SetColor(true, true, true, true);
            Console::WriteLine("Use of time: %d", c);

            // step2 LR0
            Rule::Context context;

            // String Begin
            Rule quotationMarks = Rule('\"', &context);
            Rule ruleString = quotationMarks + *!quotationMarks + quotationMarks;
            ruleString.buildDFA();
            ruleString.setShowName("\"{String}\"");
            // String End

            // Token Start
            Rule ruleToken("%token", &context);
            ruleToken.buildDFA();
            ruleToken.setShowName("%token");
            // Token End

            // Start Start
            Rule ruleStart("%start", &context);
            ruleStart.buildDFA();
            ruleStart.setShowName("%start");
            // Start End

            // Digit Start
            Rule _0('0', &context);
            Rule _9('9', &context);
            Rule _0_9  = _0 - _9;
            Rule ruleDigit = +_0_9;
            // Digit End

            // Letter Start
            Rule _('_', &context);
            Rule _a('a', &context);
            Rule _z('z', &context);
            Rule _A('A', &context);
            Rule _Z('Z', &context);
            Rule _a_z = _a - _z;
            Rule _A_Z = _A - _Z;
            Rule ruleLetter = ((+_ + ruleDigit) |
                (+(_ | _a_z | _A_Z))) +
                *(_ | ruleDigit | _a_z | _A_Z);
            ruleLetter.buildDFA();
            ruleLetter.setShowName("\"{Letter}\"");
            // Letter End

            // Arrow Start
            Rule ruleArrow("->", &context);
            ruleArrow.buildDFA();
            ruleArrow.setShowName("->");
            // Arrow End

            // Or Start
            Rule ruleOr('|', &context);
            ruleOr.buildDFA();
            ruleOr.setShowName("|");
            // Or End

            // Semicolon Start
            Rule ruleSemicolon(';', &context);
            ruleSemicolon.buildDFA();
            ruleSemicolon.setShowName(";");
            // Semicolon End

            vector<Production::Item> v;

            Production::Item itemStrings("strings");
            v.push_back(itemStrings);
            v.push_back(ruleString);
            Production productionStrings_String(itemStrings, v);              // string -> string "{String}"
            Production productionStrings_RuleString(itemStrings, ruleString); // string -> "{String}"
            v.clear();

            Production::Item itemOneProductionRight("oneProductionRight");
            v.push_back(itemOneProductionRight);
            v.push_back(ruleLetter);
            Production productionOneProductionRight_Letter(itemOneProductionRight, v); // oneProductionRight -> oneProductionRight "{Letter}"
            v.pop_back();
            v.push_back(ruleString);
            Production productionOneProductionRight_String(itemOneProductionRight, v); // oneProductionRight -> oneProductionRight "{String}"
            v.clear();
            Production productionOneProductionRight_RuleLetter(itemOneProductionRight, ruleLetter); // oneProductionRight -> "{Letter}"
            Production productionOneProductionRight_RuleString(itemOneProductionRight, ruleString); // oneProductionRight -> "{String}"

            Production::Item itemSomeProductionRight("someProductionRight");
            v.push_back(itemSomeProductionRight);
            v.push_back(ruleOr);
            v.push_back(itemOneProductionRight);
            Production productionSomeProductionRight_Or_OneProductionRight(itemSomeProductionRight, v);                   // someProductionRight -> someProductionRight "|" oneProductionRight
            Production productionSomeProductionRight_OneProductionRight(itemSomeProductionRight, itemOneProductionRight); // someProductionRight -> oneProductionRight
            v.clear();

            Production::Item itemToken("token");
            v.push_back(ruleToken);
            v.push_back(itemStrings);
            Production productionToken(itemToken, v); // token -> "%token" strings
            v.clear();

            Production::Item itemSomeTokens("someTokens");
            v.push_back(itemSomeTokens);
            v.push_back(itemToken);
            Production productionSomeTokens_Token(itemSomeTokens, v);             // someTokens -> someTokens token
            v.clear();
            Production productionSomeTokens_ItemToken(itemSomeTokens, itemToken); // someTokens -> token

            Production::Item itemProduction("production");
            v.push_back(ruleLetter);
            v.push_back(ruleArrow);
            v.push_back(itemSomeProductionRight);
            v.push_back(ruleSemicolon);
            Production productionProduction(itemProduction, v); // production -> "{Letter}" "->" someProductionRight ";"
            v.clear();

            Production::Item itemSomeProductions("someProductions");
            v.push_back(itemSomeProductions);
            v.push_back(itemProduction);
            Production productionSomeProductions_ItemProduction(itemSomeProductions, v);          // someProductions -> someProductions production
            v.clear();
            Production productionSomeProductions_Production(itemSomeProductions, itemProduction); // someProductions -> production

            Production::Item itemStart("start");
            v.push_back(itemSomeTokens);
            v.push_back(ruleStart);
            v.push_back(ruleLetter);
            v.push_back(itemSomeProductions);
            Production productionStart_HasToken(itemStart, v);   // start -> someTokens "%start" "{Letter}" someProductions
            v.clear();
            v.push_back(ruleStart);
            v.push_back(ruleLetter);
            v.push_back(itemSomeProductions);
            Production productionStart_HasntToken(itemStart, v); // start -> "%start" "{Letter}" someProductions
            v.clear();

            vector<Production> productions;
            // strings -> strings "{String}"
            productions.push_back(productionStrings_String);
            // strings -> "{String}"
            productions.push_back(productionStrings_RuleString);
            // oneProductionRight -> oneProductionRight "{Letter}"
            productions.push_back(productionOneProductionRight_Letter);
            // oneProductionRight -> oneProductionRight "{String}"
            productions.push_back(productionOneProductionRight_String);
            // oneProductionRight -> "{Letter}"
            productions.push_back(productionOneProductionRight_RuleLetter);
            // oneProductionRight -> "{String}"
            productions.push_back(productionOneProductionRight_RuleString);
            // someProductionRight -> someProductionRight "|" oneProductionRight
            productions.push_back(productionSomeProductionRight_Or_OneProductionRight);
            // someProductionRight -> oneProductionRight
            productions.push_back(productionSomeProductionRight_OneProductionRight);
            // token -> "%token" strings
            productions.push_back(productionToken);
            // someTokens -> someTokens token
            productions.push_back(productionSomeTokens_ItemToken);
            // someTokens -> token
            productions.push_back(productionSomeTokens_Token);
            // production -> "{Letter}" "->" someProductionRight ";"
            productions.push_back(productionProduction);
            // someProductions -> someProductions production
            productions.push_back(productionSomeProductions_ItemProduction);
            // someProductions -> production
            productions.push_back(productionSomeProductions_Production);
            // start -> someTokens "%start" "{Letter}" someProductions
            productions.push_back(productionStart_HasToken);
            // start -> "%start" "{Letter}" someProductions
            productions.push_back(productionStart_HasntToken);

            LR0 lr0(productions, itemStart);
            c = clock();
            lr0.make();
            c = clock() - c;
            lr0.print("LR0.txt");

            Console::SetColor(false, true, false, true);
            Console::WriteLine("Make LR(0) State Machine Finish ...");
            Console::SetColor(true, true, true, true);
            Console::WriteLine("Use of time: %d", c);

            LALR1 lalr1(lr0);
            c = clock();
            lalr1.make();
            c = clock() - c;
            lalr1.print("LALR1.txt");

            Console::SetColor(false, true, false, true);
            Console::WriteLine("Make LALR(1) State Machine Finish ...");
            Console::SetColor(true, true, true, true);
            Console::WriteLine("Use of time: %d", c);
        }
        catch (const error<string>& e)
        {
            Console::SetColor(true, false, false, true);
            Console::WriteLine(e.description);
        }
        Console::SetColor(true, true, true, false);
    }
    return 0;
}
