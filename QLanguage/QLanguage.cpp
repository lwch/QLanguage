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

    // S¡úa(+|-)A
    // A¡úb(*|/)B
    // B¡úc|S
    Production::Item start;
    Production::Item A, B;
    vector<Production::Item> v;
    v.push_back(Production::Item(Rule("a+", &context)));
    v.push_back(A);
    Production p1(start, v);

    v.clear();
    v.push_back(Production::Item(Rule("a-", &context)));
    v.push_back(A);
    Production p2(start, v);

    v.clear();
    v.push_back(Production::Item(Rule("b*", &context)));
    v.push_back(B);
    Production p3(A, v);

    v.clear();
    v.push_back(Production::Item(Rule("b/", &context)));
    v.push_back(B);
    Production p4(A, v);

    Production p5(B, Production::Item(Rule('c', &context)));

    Production p6(B, start);

    vector<Production> productions;
    productions.push_back(p1);
    productions.push_back(p2);
    productions.push_back(p3);
    productions.push_back(p4);
    productions.push_back(p5);
    productions.push_back(p6);

    LR0 lr0(productions, start);
    lr0.make();

    lr0.print();

    return 0;
}
