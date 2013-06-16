/********************************************************************
	created:	2013/05/06
	created:	6:5:2013   22:43
	filename: 	\Calculator\Calculator.cpp
	file path:	\Calculator
	file base:	Calculator
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../QCore/Library/fstream.h"

#include "../QLanguage/Lexer/Lexer.h"
#include "../QLanguage/Parser/Production.h"

#include "../QLanguage/Parser/ParserTable.h"
#include "Parser/Parser.h"

#include <stdio.h>

using namespace QLanguage;
using namespace QLanguage::Library;

int main()
{
#ifdef X86
    fstream fs("Calculator.x86.ParserTable", fstream::in | fstream::binary);
#else
    fstream fs("Calculator.x64.ParserTable", fstream::in | fstream::binary);
#endif
    string str(fs.size() + 1);

    fs.readAll(const_cast<char*>(str.c_str()), str.capacity());
    const_cast<char*>(str.c_str())[fs.size()] = 0;
    str.setsize(fs.size());
    fs.close();

    ParserTable parserTable;
    parserTable.loadFromData(str.c_str(), str.size());

    string expression;
    cout << "Please input expression:" << endl;
    cin >> expression;

    Lexer lexer;
    if (lexer.parse(expression))
    {
        Parser parser(parserTable.rules);
        try
        {
            if (!parserTable.parse(lexer.result, &parser)) cerr << "Parser error";
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
    }
    else cerr << "Lexer error";

    return 0;
}
