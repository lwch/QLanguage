/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   16:26
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_DeclareName.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_DeclareName
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_DeclareName.h"

namespace QLanguage
{
    SyntaxTree_DeclareName::SyntaxTree_DeclareName(const string& name, bool bArray) : parent(sizeof(SyntaxTree_DeclareName)), name(name), idx(inc()), bArray(bArray)
    {
    }

    SyntaxTree_DeclareName::~SyntaxTree_DeclareName()
    {
    }

    void SyntaxTree_DeclareName::print(ostream& stream, uint indent)const
    {
    }

    // declare_desc -> declare_desc "," "{Letter}"
    // declare_desc -> type_desc "{Letter}"
    bool Parser::reduceDeclare48(ushort i)
    {
        SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
        construct(pDeclareName, shifts.top(), false);

        context.data.insert(pDeclareName);

        syntaxTreeStack.top()->pushChild(pDeclareName);

        shifts.pop();
        if (i == DECLARE_DESC_DECLARE_DESC_LETTER) shifts.pop();
        return true;
    }
}
