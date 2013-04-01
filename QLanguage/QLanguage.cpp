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

    Production::Item a, b;
    a = Rule('a', &context);
    b = Rule('b', &context);

    // S¡úaS
    // S¡úbS
    // S¡úa
    Production::Item start;
    vector<Production::Item> v;
    v.push_back(Production::Item(a));
    v.push_back(start);
    Production p1(start, v);

    v.clear();
    v.push_back(Production::Item(b));
    v.push_back(start);
    Production p2(start, v);

    Production p3(start, a);

    vector<Production> productions;
    productions.push_back(p1);
    productions.push_back(p2);
    productions.push_back(p3);

    LR0 lr0(productions, start);
    lr0.make();

    lr0.print();

    return 0;
}
