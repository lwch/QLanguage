/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   21:03
	filename: 	\QParserGenerator\Parser\Parser.cpp
	file path:	\QParserGenerator\Parser
	file base:	Parser
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "Parser.h"

namespace QLanguage
{
    Parser::Parser(const vector<Production>& productions) : BasicParser(productions)
    {
#ifdef _DEBUG
        stream.open("ParserResult.txt", fstream::out | fstream::binary);
#endif
        // String Begin
        Rule quotationMarks = Rule('\"', &context);
        Rule ruleString = quotationMarks + *!quotationMarks + quotationMarks;
        ruleString.buildDFA();
#ifdef _DEBUG
        ruleString.setShowName("\"{String}\"");
#endif
        Production::Item itemString(ruleString);
        // String End

        // Digit Start
        Rule _0('0', &context);
        Rule _9('9', &context);
        Rule _0_9  = _0 - _9;
        Rule ruleDigit = +_0_9;
        ruleDigit.buildDFA();
#ifdef _DEBUG
        ruleDigit.setShowName("\"{Digit}\"");
#endif
        Production::Item itemDigit(ruleDigit);
        // Digit End

        // Real Start
        Rule _point('.', &context);
        Rule ruleReal = *_0_9 + _point + +_0_9;
        ruleReal.buildDFA();
#ifdef _DEBUG
        ruleReal.setShowName("\"{Real}\"");
#endif
        Production::Item itemReal(ruleReal);
        // Real End

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
#ifdef _DEBUG
        ruleLetter.setShowName("\"{Letter}\"");
#endif
        Production::Item itemLetter(ruleLetter);
        // Letter End

        vts.push_back(pair<string, Production::Item>("{String}", itemString));
        vts.push_back(pair<string, Production::Item>("{Digit}",  itemDigit));
        vts.push_back(pair<string, Production::Item>("{Real}",   itemReal));
        vts.push_back(pair<string, Production::Item>("{Letter}", itemLetter));
    }

    Parser::~Parser()
    {
    }

    bool Parser::shift(const string& s)
    {
        shifts.push_back(s);
#ifdef _DEBUG
        stream << "shift by: " << s << endl;
#endif
        return true;
    }

    bool Parser::reduce(ushort i)
    {
#ifdef _DEBUG
        stream << "reduced by: ";
        inputProductions[i].print(stream);
#endif
        switch (i)
        {
        case 0:  // begin -> start
            break;
        case 1:  // strings -> strings "{String}"
            return reduceStrings1();
        case 2:  // strings -> "{String}"
            return reduceStrings2();
        case 3:  // vs -> vs "{Letter}"
            return reduceVs1();
        case 4:  // vs -> vs "{String}"
            return reduceVs2();
        case 5:  // vs -> "{Letter}"
            return reduceVs3();
        case 6:  // vs -> "{String}"
            return reduceVs4();
        case 7:  // option -> "[" vs "]"
            return reduceOption();
        case 8:  // oneProductionRight -> oneProductionRight vs
            return reduceRight1();
        case 9:  // oneProductionRight -> oneProductionRight option
            return reduceRight2();
        case 10:  // oneProductionRight -> vs
            return reduceRight3();
        case 11:  // someProductionRight -> someProduction "|" oneProductionRight
            return reduceSomeRight1();
        case 12:  // someProductionRight -> oneProductionRight
            return reduceSomeRight2();
        case 13:  // token -> "%" "token" strings ";"
            return reduceToken();
        case 14: // someTokens -> token
            break;
        case 15: // someTokens -> someTokens token
            break;
        case 16: // production -> "{Letter}" "-" ">" someProductionRight ";"
            return reduceProduction();
        case 17: // someProductions -> someProductions production
            break;
        case 18: // someProductions -> production
            break;
        case 19: // start -> someTokens "%" "start" "{Letter}" ";" someProductions
            return reduceAll();
        case 20: // start -> "%" "start" "{Letter}" ";" someProductions
            return reduceAll();
        }
        return true;
    }

    void Parser::printRules(const string& path)
    {
        fstream fs(path, fstream::out | fstream::text);
        for (size_t i = 0, m = inputProductions.size(); i < m; ++i)
        {
            fs << i << " : ";
            inputProductions[i].print(fs);
        }
    }

    // start -> someTokens "%" "start" "{Letter}" ";" someProductions
    // start -> "%" "start" "{Letter}" ";" someProductions
    bool Parser::reduceAll()
    {
        shifts.pop_back();
        long idx = indexOfVN(shifts.back());
        Production::Item* pItem = NULL;
        if (idx == -1)
        {
            vns.push_back(pair<string, Production::Item>(shifts.back(), Production::Item(
#if defined(_DEBUG) && DEBUG_LEVEL == 3
                          shifts.back()
#endif
                          )));
            pItem = &vns.back().second;
        }
        else pItem = &vns[idx].second;
        lalr1.setStart(pItem);
        lalr1.setProductions(productions);
        lalr1.make();
        fstream s("QLanguage.lalr1", fstream::out | fstream::text);
        lalr1.print(s);
        lalr1.output("QLanguage.parsertable");
        shifts.pop_back();
        shifts.pop_back();
        shifts.pop_back();
        return true;
    }

    // strings -> strings "{String}"
    bool Parser::reduceStrings1()
    {
        string s = string(shifts.back(), 1, shifts.back().size() - 2);
        shifts.pop_back();
        Rule r(s.c_str(), &context);
        r.buildDFA();
#if defined(_DEBUG) && DEBUG_LEVEL == 3
        r.setShowName(s);
#endif
        vts.push_back(pair<string, Production::Item>(s, r));
        return true;
    }

    // strings -> "{String}"
    bool Parser::reduceStrings2()
    {
        string s = string(shifts.back(), 1, shifts.back().size() - 2);
        shifts.pop_back();
        Rule r(s.c_str(), &context);
        r.buildDFA();
#if defined(_DEBUG) && DEBUG_LEVEL == 3
        r.setShowName(s);
#endif
        vts.push_back(pair<string, Production::Item>(s, r));
        return true;
    }

    // vs -> vs "{Letter}"
    bool Parser::reduceVs1()
    {
        long idx = indexOfVN(shifts.back());
        Production::Item* pItem = NULL;
        if (idx == -1)
        {
            vns.push_back(pair<string, Production::Item>(shifts.back(), Production::Item(
#if defined(_DEBUG) && DEBUG_LEVEL == 3
                          shifts.back()
#endif
                          )));
            pItem = &vns.back().second;
        }
        else pItem = &vns[idx].second;
        vs.push_back(*pItem);
        shifts.pop_back();
        return true;
    }

    // vs -> vs "{String}"
    bool Parser::reduceVs2()
    {
        long idx = indexOfVT(string(shifts.back(), 1, shifts.back().size() - 2));
        vs.push_back(vts[idx].second);
        shifts.pop_back();
        return true;
    }

    // vs -> "{Letter}"
    bool Parser::reduceVs3()
    {
        vs.clear();
        long idx = indexOfVN(shifts.back());
        Production::Item* pItem = NULL;
        if (idx == -1)
        {
            vns.push_back(pair<string, Production::Item>(shifts.back(), Production::Item(
#if defined(_DEBUG) && DEBUG_LEVEL == 3
                shifts.back()
#endif
                )));
            pItem = &vns.back().second;
        }
        else pItem = &vns[idx].second;
        vs.push_back(*pItem);
        shifts.pop_back();
        return true;
    }

    // vs -> "{String}"
    bool Parser::reduceVs4()
    {
        vs.clear();
        long idx = indexOfVT(string(shifts.back(), 1, shifts.back().size() - 2));
        vs.push_back(vts[idx].second);
        shifts.pop_back();
        return true;
    }

    // option -> "[" vs "]"
    bool Parser::reduceOption()
    {
        shifts.pop_back();
        shifts.pop_back();
        return true;
    }

    // oneProductionRight -> oneProductionRight vs
    bool Parser::reduceRight1()
    {
        for (vector<vector<Production::Item> >::iterator i = oneProductionRights.begin(), m = oneProductionRights.end(); i != m; ++i)
        {
            i->add(vs);
        }
        return true;
    }

    // oneProductionRight -> oneProductionRight option
    bool Parser::reduceRight2()
    {
        for (vector<vector<Production::Item> >::iterator i = oneProductionRights.begin(), m = oneProductionRights.end(); i != m; ++i)
        {
            oneProductionRights.push_back(*i);
            oneProductionRights.back().add(vs);
        }
        return true;
    }

    // oneProductionRight -> vs
    bool Parser::reduceRight3()
    {
        oneProductionRights.clear();
        oneProductionRights.push_back(vs);
        return true;
    }

    // someProductionRight -> someProductionRight "|" oneProductionRight
    bool Parser::reduceSomeRight1()
    {
        shifts.pop_back();
        for (vector<vector<Production::Item> >::const_iterator i = oneProductionRights.begin(), m = oneProductionRights.end(); i != m; ++i)
        {
            rights.push_back(*i);
        }
        return true;
    }

    // someProductionRight -> oneProductionRight
    bool Parser::reduceSomeRight2()
    {
        rights.clear();
        for (vector<vector<Production::Item> >::const_iterator i = oneProductionRights.begin(), m = oneProductionRights.end(); i != m; ++i)
        {
            rights.push_back(*i);
        }
        return true;
    }

    // production -> "{Letter}" "-" ">" someProductionRight ";"
    bool Parser::reduceProduction()
    {
        shifts.pop_back();
        shifts.pop_back();
        shifts.pop_back();
        long idx = indexOfVN(shifts.back());
        Production::Item* pItem = NULL;
        if (idx == -1)
        {
            vns.push_back(pair<string, Production::Item>(shifts.back(), Production::Item(
#if defined(_DEBUG) && DEBUG_LEVEL == 3
                          shifts.back()
#endif
                          )));
            pItem = &vns.back().second;
        }
        else pItem = &vns[idx].second;
        for (vector<vector<Production::Item> >::const_iterator i = rights.begin(), m = rights.end(); i != m; ++i)
        {
            Production p(*pItem, *i);
            productions.push_back(p);
        }
        shifts.pop_back();
        rights.clear();
        return true;
    }

    // token -> "%" "token" strings ";"
    bool Parser::reduceToken()
    {
        shifts.pop_back();
        shifts.pop_back();
        shifts.pop_back();
        return true;
    }

    long Parser::indexOfVN(const string& s)
    {
        for (size_t i = 0, m = vns.size(); i < m; ++i)
        {
            if (vns[i].first == s) return (long)i;
        }
        return -1;
    }

    long Parser::indexOfVT(const string& s)
    {
        for (size_t i = 0, m = vts.size(); i < m; ++i)
        {
            if (vts[i].first == s) return (long)i;
        }
        return -1;
    }
}
