#include "Lexer/Lexer.h"
#include "Parser/LR0.h"

#include <time.h>
#include <stdio.h>

using namespace QLanguage;

int main()
{
    clock_t t = clock();
    Lexer lexer;
    printf("Make Lexer use of time: %ld\n", clock() - t);
    string input = "int main()\n"
                   "{\n"
                   "    return 0;"
                   "}\n";
    lexer.parse(input);
    printf("Total time: %ld\n", clock() - t);

    Rule::Context context;

    Production::Item start;
    Production p(start, Production::Item(Rule('a', &context)));
    vector<Production> productions;
    productions.push_back(p);
    LR0 lr0(productions, start);
    lr0.make();

    return 0;
}
