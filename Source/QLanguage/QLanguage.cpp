#include "Lexer/Lexer.h"
#include "Parser/LALR1.h"

#include <time.h>
#include <stdio.h>

using namespace QLanguage;

int main()
{
//     clock_t t = clock();
//     Lexer lexer;
//     cout << string::format("Make Lexer use of time: %ld", clock() - t) << endl;
//     string input = "a a a";
//     lexer.parse(input);
//     cout << string::format("Total time: %ld", clock() - t) << endl;
    list<Lexer::Token> l;
    l.push_back(Lexer::Token(Lexer::Token::String, "a"));
    l.push_back(Lexer::Token(Lexer::Token::String, "a"));
    l.push_back(Lexer::Token(Lexer::Token::String, "a"));

    Rule::Context context;

    Rule ruleA('a', &context);
    ruleA.buildDFA();
#ifdef _DEBUG
    ruleA.setShowName("a");
#endif
    Production::Item a(ruleA);

    // Z¡úZa
    // Z¡úa
#if defined(_DEBUG) && DEBUG_LEVEL == 3
    Production::Item Z("A");
#else
    Production::Item Z;
#endif
    vector<Production::Item> v;
    v.push_back(Z);
    v.push_back(a);
    Production p1(Z, v);

    Production p2(Z, a);

    vector<Production> productions;
    productions.push_back(p1);
    productions.push_back(p2);

    LALR1 lalr1(productions, Z);
    lalr1.make();

    lalr1.print(cout);
// 
//     lalr1.parse(l);

    return 0;
}
