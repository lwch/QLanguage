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
#include "SyntaxTree_Block.h"
#include "SyntaxTree_Exp.h"
#include "SyntaxTree_Return.h"

namespace QLanguage
{
    SyntaxTree_Return::SyntaxTree_Return(SyntaxTree_Base* pContent)
        : parent(sizeof(SyntaxTree_Return))
        , pContent(pContent)
    {
    }

    SyntaxTree_Return::~SyntaxTree_Return()
    {
    }

    void SyntaxTree_Return::print(ostream& stream, uint indent)const
    {
        stream << "return ";
        pContent->print(stream, indent + parent::indent);
        if (pContent->type() == "SyntaxTree_Exp") stream << ';';
    }

    // return_desc -> "return" block
    bool Parser::reduceReturnBlock()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();

        SyntaxTree_Return* pReturn = allocator<SyntaxTree_Return>::allocate();
        construct(pReturn, syntaxTreeStack.top());

        context.data.insert(pReturn);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pReturn);

        return true;
    }

    // return_desc -> "return" exp ";"
    bool Parser::reduceReturnExp()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Return* pReturn = allocator<SyntaxTree_Return>::allocate();
        construct(pReturn, syntaxTreeStack.top());

        context.data.insert(pReturn);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pReturn);

        return true;
    }
}
