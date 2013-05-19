/********************************************************************
	created:	2013/05/19
	created:	19:5:2013   20:23
	filename: 	\QLanguage\QLanguage.cpp
	file path:	\QLanguage
	file base:	QLanguage
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "Lexer/Lexer.h"
#include "Parser/ParserTable.h"
#include "Parser/Parser.h"

#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

using namespace QLanguage;

int main(int argv, char* args[])
{
    string path;
#ifdef _DEBUG
    if (argv < 2)
    {
        char currentPath[MAX_PATH] = {0};
        getcwd(currentPath, MAX_PATH);
        path = currentPath;
        path += "/test.txt";
    }
#else
    if (argv < 2)
    {
        cout << "please input file" << endl;
        return 1;
    }
#endif
    else path = args[1];

    fstream fs("QLanguage.ParserTable", fstream::in | fstream::binary);
    string str(fs.size() + 1);

    fs.readAll(const_cast<char*>(str.c_str()), str.capacity());
    const_cast<char*>(str.c_str())[fs.size()] = 0;
    str.setsize(fs.size());
    fs.close();

    ParserTable parserTable;
    parserTable.loadFromData(str.c_str(), str.size());

    str.clear();
    fs.open(path.c_str(), fstream::in | fstream::text);
    str.reserve(fs.size() + 1);

    fs.readAll(const_cast<char*>(str.c_str()), str.capacity());
    const_cast<char*>(str.c_str())[fs.size()] = 0;
    str.setsize(fs.size());
    fs.close();

    Lexer lexer;
    clock_t c = clock();
    if (lexer.parse(str))
    {
        c = clock() - c;
        cout.setColor(cout.lightWith(stdstream::green));
        cout << "Lexer Parse Finish ..." << endl;
        cout.setColor(cout.lightWith(stdstream::white));
        cout << string::format("Use of time: %d", c) << endl;

        Parser parser(parserTable.rules);
        c = clock();
        if (parserTable.parse(lexer.result, &parser))
        {
            c = clock() - c;
            cout.setColor(cout.lightWith(stdstream::green));
            cout << "Parser Parse Finish ..." << endl;
            cout.setColor(cout.lightWith(stdstream::white));
            cout << string::format("Use of time: %d", c) << endl;
        }
        else cerr << "parser error" << endl;
    }
    else cerr << "lexer parse error" << endl;

    return 0;
}
