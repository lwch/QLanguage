#ifndef _PARSER_H_
#define _PARSER_H_

#include "../../QLanguage/Parser/BasicParser.h"

namespace QLanguage
{
    class Parser : public BasicParser
    {
    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);
    protected:
        vector<string> shifts;
    };
}

#endif
