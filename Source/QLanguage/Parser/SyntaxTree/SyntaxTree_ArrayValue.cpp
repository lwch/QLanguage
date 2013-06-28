#include "../Parser.h"
#include "SyntaxTree_ArrayValue.h"

namespace QLanguge
{
    SyntaxTree_ArrayValue::SyntaxTree_ArrayValue(SyntaxTree_Values* pValues)
        : parent(sizeof(SyntaxTree_ArrayValue))
        , pValues(pValues)
    {
    }

    SyntaxTree_ArrayValue::~SyntaxTree_ArrayValue()
    {
    }

    void SyntaxTree_ArrayValue::print(ostream& stream, uint indent)const
    {
    }
}
