#include "Lexer/Lexer.h"

#include <time.h>
#include <stdio.h>

using namespace QLanguage;

int main()
{
    clock_t t = clock();
    Lexer lexer;
    printf("Make Lexer use of time: %ld", clock() - t);
    return 0;
}
