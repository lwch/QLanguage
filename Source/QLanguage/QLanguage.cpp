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
#include <limits.h>
#endif

using namespace QLanguage;

int main(int argv, char* args[])
{
    string path;
#ifdef _DEBUG
    if (argv < 2)
    {
#ifdef WIN32
        char currentPath[MAX_PATH] = {0};
        getcwd(currentPath, MAX_PATH);
#else
        char currentPath[PATH_MAX] = {0};
        getcwd(currentPath, PATH_MAX);
#endif
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

#ifdef X86
    fstream fs("QLanguage.x86.ParserTable", fstream::in | fstream::binary);
#else
    fstream fs("QLanguage.x64.ParserTable", fstream::in | fstream::binary);
#endif
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

    try
    {
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
    }
    catch (const error<const char*>& e)
    {
        e.print();
    }
    catch (const error<char*>& e)
    {
        e.print();
    }
    catch (const error<string>& e)
    {
        e.print();
    }
    catch (...)
    {
        cerr << "unknown error" << endl;
    }

    return 0;
}
