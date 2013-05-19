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
#include "../QCore/Library/fstream.h"

#include "../QLanguage/Lexer/Lexer.h"
#include "../QLanguage/Parser/LALR1.h"
#include "Parser/Parser.h"

#include <stdio.h>
#include <time.h>
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

using namespace QLanguage;
using namespace QLanguage::Library;

int main(int argv, char* args[])
{
    string path, outputPath;
#ifdef _DEBUG
    if (argv < 3)
    {
        char currentPath[MAX_PATH] = {0};
        getcwd(currentPath, MAX_PATH);
        path = currentPath;
        path += "/QLanguage.txt";
        outputPath = currentPath;
        outputPath += "/QLanguage.ParserTable";
        //path = "C:\\QLanguage\\Source\\GeneratorFiles\\NewQLanguage.txt";
        //path = "C:\\QLanguage\\Source\\GeneratorFiles\\QLanguage.txt";
        //path = "E:\\QLanguage\\Source\\GeneratorFiles\\NewQLanguage.txt";
    }
    else
    {
        path = args[1];
        outputPath = args[2];
    }
#else
    if (argv < 3) cout << "Please input Generator File and Output Path";
    else
    {
        path = args[1];
        outputPath = args[2];
    }
#endif
    if (path.size() && outputPath.size())
    {
        try
        {
            // step1 lexer
            fstream fs(path, fstream::in | fstream::binary);
            string str(fs.size() + 1);

            fs.readAll(const_cast<char*>(str.c_str()), str.capacity());
            const_cast<char*>(str.c_str())[fs.size()] = 0;
            str.setsize(fs.size());
            fs.close();

            Lexer lexer;
            clock_t c = clock();
            lexer.parse(str);
            c = clock() - c;

            cout.setColor(cout.lightWith(stdstream::green));
            cout << "Lexer Finish ..." << endl;
            cout.setColor(cout.lightWith(stdstream::white));
            cout << string::format("Use of time: %d", c) << endl;

            // step2 LR0
            Rule::Context context;

            // String Begin
            Rule quotationMarks = Rule('\"', &context);
            Rule ruleString = quotationMarks + *!quotationMarks + quotationMarks;
            ruleString.buildDFA();
            ruleString.setShowName("\"{String}\"");
            Production::Item itemString(ruleString);
            // String End

            // Token Start
            Rule rulePrecent("%", &context);
            rulePrecent.buildDFA();
            rulePrecent.setShowName("\"%\"");
            Production::Item itemRulePrecent(rulePrecent);

            Rule ruleToken("token", &context);
            ruleToken.buildDFA();
            ruleToken.setShowName("\"token\"");
            Production::Item itemRuleToken(ruleToken);
            // Token End

            // Start Start
            Rule ruleStart("start", &context);
            ruleStart.buildDFA();
            ruleStart.setShowName("\"start\"");
            Production::Item itemRuleStart(ruleStart);
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
            Production::Item itemLetter(ruleLetter);
            // Letter End

            // Arrow Start
            Rule ruleSub('-', &context);
            ruleSub.buildDFA();
            ruleSub.setShowName("\"-\"");
            Production::Item itemRuleSub(ruleSub);

            Rule ruleMore('>', &context);
            ruleMore.buildDFA();
            ruleMore.setShowName("\">\"");
            Production::Item itemRuleMore(ruleMore);
            // Arrow End

            // Or Start
            Rule ruleOr('|', &context);
            ruleOr.buildDFA();
            ruleOr.setShowName("\"|\"");
            Production::Item itemRuleOr(ruleOr);
            // Or End

            // Semicolon Start
            Rule ruleSemicolon(';', &context);
            ruleSemicolon.buildDFA();
            ruleSemicolon.setShowName("\";\"");
            Production::Item itemRuleSemicolon(ruleSemicolon);
            // Semicolon End

            // SquareBrackets Start
            Rule ruleSquareBrackets1('[', &context);
            Rule ruleSquareBrackets2(']', &context);
            ruleSquareBrackets1.buildDFA();
            ruleSquareBrackets2.buildDFA();
            ruleSquareBrackets1.setShowName("\"[\"");
            ruleSquareBrackets2.setShowName("\"]\"");
            Production::Item itemSquareBrackets1(ruleSquareBrackets1);
            Production::Item itemSquareBrackets2(ruleSquareBrackets2);
            // SquareBrackets End

            vector<Production::Item> v;

            Production::Item itemStrings("strings");
            v.push_back(itemStrings);
            v.push_back(itemString);
            Production productionStrings_String(itemStrings, v);              // string -> string "{String}"
            Production productionStrings_RuleString(itemStrings, itemString); // string -> "{String}"
            v.clear();

            Production::Item itemVs("vs");
            v.push_back(itemVs);
            v.push_back(itemLetter);
            Production productionVs_Letter(itemVs, v);
            v.pop_back();
            v.push_back(itemString);
            Production productionVs_String(itemVs, v);
            v.clear();
            Production productionVs_RuleLetter(itemVs, itemLetter);
            Production productionVs_RuleString(itemVs, itemString);

            Production::Item itemOption("option");
            v.push_back(itemSquareBrackets1);
            v.push_back(itemVs);
            v.push_back(itemSquareBrackets2);
            Production productionOption(itemOption, v);
            v.clear();

            Production::Item itemOneProductionRight("oneProductionRight");
            v.push_back(itemOneProductionRight);
            v.push_back(itemOption);
            Production productionOneProductionRight_Options(itemOneProductionRight, v); // oneProductionRight -> vs oneProductionRightBack
            v.pop_back();
            v.push_back(itemVs);
            Production productionOneProductionRight_Vss(itemOneProductionRight, v);
            v.clear();
            Production productionOneProductionRight_Option(itemOneProductionRight, itemOption);
            Production productionOneProductionRight_Vs(itemOneProductionRight, itemVs);

            Production::Item itemSomeProductionRight("someProductionRight");
            v.push_back(itemSomeProductionRight);
            v.push_back(itemRuleOr);
            v.push_back(itemOneProductionRight);
            Production productionSomeProductionRight_Or_OneProductionRight(itemSomeProductionRight, v);                   // someProductionRight -> someProductionRight "|" oneProductionRight
            Production productionSomeProductionRight_OneProductionRight(itemSomeProductionRight, itemOneProductionRight); // someProductionRight -> oneProductionRight
            v.clear();

            Production::Item itemToken("token");
            v.push_back(itemRulePrecent);
            v.push_back(itemRuleToken);
            v.push_back(itemStrings);
            v.push_back(itemRuleSemicolon);
            Production productionToken(itemToken, v); // token -> "%" "token" strings ";"
            v.clear();

            Production::Item itemSomeTokens("someTokens");
            v.push_back(itemSomeTokens);
            v.push_back(itemToken);
            Production productionSomeTokens_Token(itemSomeTokens, v);             // someTokens -> someTokens token
            v.clear();
            Production productionSomeTokens_ItemToken(itemSomeTokens, itemToken); // someTokens -> token

            Production::Item itemProduction("production");
            v.push_back(itemLetter);
            v.push_back(itemRuleSub);
            v.push_back(itemRuleMore);
            v.push_back(itemSomeProductionRight);
            v.push_back(itemRuleSemicolon);
            Production productionProduction(itemProduction, v); // production -> "{Letter}" "-" ">" someProductionRight ";"
            v.clear();

            Production::Item itemSomeProductions("someProductions");
            v.push_back(itemSomeProductions);
            v.push_back(itemProduction);
            Production productionSomeProductions_ItemProduction(itemSomeProductions, v);          // someProductions -> someProductions production
            v.clear();
            Production productionSomeProductions_Production(itemSomeProductions, itemProduction); // someProductions -> production

            Production::Item itemStart("start");
            v.push_back(itemSomeTokens);
            v.push_back(itemRulePrecent);
            v.push_back(itemRuleStart);
            v.push_back(itemLetter);
            v.push_back(itemRuleSemicolon);
            v.push_back(itemSomeProductions);
            Production productionStart_HasToken(itemStart, v);   // start -> someTokens "%" "start" "{Letter}" ";" someProductions
            v.clear();
            v.push_back(itemRulePrecent);
            v.push_back(itemRuleStart);
            v.push_back(itemLetter);
            v.push_back(itemRuleSemicolon);
            v.push_back(itemSomeProductions);
            Production productionStart_HasntToken(itemStart, v); // start -> "%" "start" "{Letter}" ";" someProductions
            v.clear();

            vector<Production> productions;
            // strings -> strings "{String}"
            productions.push_back(productionStrings_String);
            // strings -> "{String}"
            productions.push_back(productionStrings_RuleString);
            // vs -> vs "{Letter}"
            productions.push_back(productionVs_Letter);
            // vs -> vs "{String}"
            productions.push_back(productionVs_String);
            // vs -> "{Letter}"
            productions.push_back(productionVs_RuleLetter);
            // vs -> "{String}"
            productions.push_back(productionVs_RuleString);
            // option -> "[" vs "]"
            productions.push_back(productionOption);
            // oneProductionRight -> vs oneProductionRightBack
            productions.push_back(productionOneProductionRight_Options);
            productions.push_back(productionOneProductionRight_Vss);
            productions.push_back(productionOneProductionRight_Option);
            productions.push_back(productionOneProductionRight_Vs);
            // someProductionRight -> someProductionRight "|" oneProductionRight
            productions.push_back(productionSomeProductionRight_Or_OneProductionRight);
            // someProductionRight -> oneProductionRight
            productions.push_back(productionSomeProductionRight_OneProductionRight);
            // token -> "%" "token" strings ";"
            productions.push_back(productionToken);
            // someTokens -> someTokens token
            productions.push_back(productionSomeTokens_ItemToken);
            // someTokens -> token
            productions.push_back(productionSomeTokens_Token);
            // production -> "{Letter}" "-" ">" someProductionRight ";"
            productions.push_back(productionProduction);
            // someProductions -> someProductions production
            productions.push_back(productionSomeProductions_ItemProduction);
            // someProductions -> production
            productions.push_back(productionSomeProductions_Production);
            // start -> someTokens "%" "start" "{Letter}" ";" someProductions
            productions.push_back(productionStart_HasToken);
            // start -> "%" "start" "{Letter}" ";" someProductions
            productions.push_back(productionStart_HasntToken);

            LALR1 lalr1(productions, itemStart);
            c = clock();
            lalr1.make();
            c = clock() - c;
//             fstream lalr1stream("LALR1.txt", fstream::out);
//             lalr1.print(lalr1stream);

            cout.setColor(cout.lightWith(stdstream::green));
            cout << "Make LALR(1) State Machine Finish ..." << endl;
            cout.setColor(cout.lightWith(stdstream::white));
            cout << string::format("Use of time: %d", c) << endl;

//             c = clock();
//             lalr1.output("QParserGenerator.ParserTable");
//             c = clock() - c;
// 
//             cout.setColor(cout.lightWith(stdstream::green));
//             cout << "Output QParserGenerator LALR(1) ParserTable Finish ..." << endl;
//             cout.setColor(cout.lightWith(stdstream::white));
//             cout << string::format("Use of time: %d", c) << endl;

            Parser parser(lalr1.rules(), outputPath);
            c = clock();
            lalr1.parse(lexer.result, &parser);
            c = clock() - c;

            cout.setColor(cout.lightWith(stdstream::green));
            cout << "Parse LALR(1) ParserTable Finish ..." << endl;
            cout.setColor(cout.lightWith(stdstream::white));
            cout << string::format("Use of time: %d", c) << endl;
        }
        catch (const error<char*>& e)
        {
            e.print();
        }
        catch (const error<const char*>& e)
        {
            e.print();
        }
        catch (const error<string>& e)
        {
            e.print();
        }
        catch (...)
        {
            cout.setColor(cout.lightWith(stdstream::red));
            cout << "unknown error" << endl;
        }
        cout.setColor(stdstream::white);
    }
    return 0;
}
