/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   20:30
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Return.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Return
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Return.h"

namespace QLanguage
{
    SyntaxTree_Return::SyntaxTree_Return(SyntaxTree_Base* pExp) : parent(sizeof(SyntaxTree_Return)), pExp(pExp)
    {
    }

    SyntaxTree_Return::~SyntaxTree_Return()
    {
    }

    void SyntaxTree_Return::print(ostream& stream, uint indent)const
    {
        stream << "return ";
        pExp->print(stream, indent);
        stream << ';';
    }

    // return_desc -> "return" exp ";"
    bool Parser::reduceReturnExp()
    {
        SyntaxTree_Return* pReturn = allocator<SyntaxTree_Return>::allocate();
        construct(pReturn, syntaxTreeStack.top());

        context.data.insert(pReturn);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pReturn);

        shifts.pop();
        shifts.pop();
        return true;
    }
}
