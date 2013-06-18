#include "../Parser.h"
#include "SyntaxTree_Do.h"

namespace QLanguage
{
    SyntaxTree_Do::SyntaxTree_Do(const SyntaxTree_Block& block, const SyntaxTree_Exp& exp)
        : parent(sizeof(SyntaxTree_Do))
        , block(block)
        , exp(exp)
    {
    }

    SyntaxTree_Do::~SyntaxTree_Do()
    {
    }

    void SyntaxTree_Do::print(ostream& stream, uint indent) const
    {
    }

    // do_desc -> "do" block "while" "(" exp ")"
    bool Parser::reduceDo()
    {
        shifts.pop();
        shifts.pop();
        shifts.pop();
        shifts.pop();
        
        SyntaxTree_Do* pDo = allocator<SyntaxTree_Do>::allocate();
        construct(pDo, dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack[1]), dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack.top()));
        
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pDo);
        return true;
    }
}
