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
    return 0;
}
