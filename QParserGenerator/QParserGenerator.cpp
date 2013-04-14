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
