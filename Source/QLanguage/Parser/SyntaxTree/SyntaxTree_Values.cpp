#include "../Parser.h"
#include "SyntaxTree_Values.h"

namespace QLanguage
{
    SyntaxTree_Values::SyntaxTree_Values(bool bTop /* = false */)
        : parent(sizeof(SyntaxTree_Values))
        , bTop(bTop)
    {
    }

    SyntaxTree_Values::~SyntaxTree_Values()
    {
    }

    void SyntaxTree_Values::print(ostream& stream, uint indent)const
    {
    }

    // values -> "{" values "}"
    bool Parser::reduceValuesTop()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Values* pValues = allocator<SyntaxTree_Values>::allocate();
        construct(pValues, true);

        context.data.insert(pValues);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValues);

        return true;
    }

    // values -> values "," value_desc
    bool Parser::reduceValues2Size()
    {
        shifts.pop();

        SyntaxTree_Values* pValues = dynamic_cast<SyntaxTree_Values*>(syntaxTreeStack[1]);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();

        return true;
    }

    // values -> value_desc
    bool Parser::reduceValues1Size()
    {
        SyntaxTree_Values* pValues = allocator<SyntaxTree_Values>::allocate();
        construct(pValues);

        context.data.insert(pValues);

        pValues->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValues);

        return true;
    }
}
