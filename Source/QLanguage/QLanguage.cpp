#include "Lexer/Lexer.h"
#include "Parser/LR0.h"
#include "Parser/LALR1.h"

#include <time.h>
#include <stdio.h>

using namespace QLanguage;

int main()
{
    clock_t t = clock();
    Lexer lexer;
    cout << string::format("Make Lexer use of time: %ld", clock() - t) << endl;
    string input = "int main()\n"
                   "{\n"
                   "    return 0;"
                   "}\n";
    lexer.parse(input);
    cout << string::format("Total time: %ld", clock() - t) << endl;

    Rule::Context context;

    Production::Item a(Rule('a', &context)), b(Rule('b', &context));

    // Z¡úBB
    // B¡úaB
    // B¡úb
    Production::Item Z("Z"), B("B");
    vector<Production::Item> v;
    v.push_back(B);
    v.push_back(B);
    Production p1(Z, v);

    v.clear();
    v.push_back(Production::Item(a));
    v.push_back(B);
    Production p2(B, v);

    Production p3(B, Production::Item(b));

    vector<Production> productions;
    productions.push_back(p1);
    productions.push_back(p2);
    productions.push_back(p3);

    LR0 lr0(productions, Z);
    lr0.make();

    lr0.print(cin);

//     LALR1 lalr1(lr0);
//     lalr1.make();
// 
//     lalr1.print("LALR1.txt");

    return 0;
}
