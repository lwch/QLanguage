/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   21:03
	filename: 	\QParserGenerator\Parser\Parser.h
	file path:	\QParserGenerator\Parser
	file base:	Parser
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_GENERATOR_PARSER_H_
#define _QLANGUAGE_GENERATOR_PARSER_H_

#include "../../QLanguage/Parser/BasicParser.h"
#include "../../QLanguage/Parser/LALR1.h"

namespace QLanguage
{
    class Parser : public BasicParser
    {
    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);

        void printRules(const string& path);
    protected:
        bool reduceAll();
        bool reduceStrings1();
        bool reduceStrings2();
        bool reduceVs1();
        bool reduceVs2();
        bool reduceVs3();
        bool reduceVs4();
        bool reduceOption();
        bool reduceRight1();
        bool reduceRight2();
        bool reduceRight3();
        bool reduceSomeRight1();
        bool reduceSomeRight2();
        bool reduceProduction();
        bool reduceToken();

        long indexOfVN(const string& s);
        long indexOfVT(const string& s);
    protected:
        LALR1                             lalr1;
        vector<Production>                productions;
        vector<string>                    shifts;
        vector<Production::Item>          vs;
        vector<vector<Production::Item> > oneProductionRights;
        vector<vector<Production::Item> > rights;
        regex::Rule::Context              context;
        vector<pair<string, Production::Item> > vns;
        vector<pair<string, Production::Item> > vts;
        Production::Item                  itemString;
        Production::Item                  itemLetter;
#ifdef _DEBUG
        fstream                           stream;
#endif
    };
}

#endif
