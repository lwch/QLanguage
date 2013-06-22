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
#include "SyntaxTree_Exp.h"
#include "SyntaxTree_DeclareName.h"

namespace QLanguage
{
    SyntaxTree_DeclareName::SyntaxTree_DeclareName(const string& name, bool bArray)
        : parent(sizeof(SyntaxTree_DeclareName))
        , name(name)
        , bArray(bArray)
        , pExp(NULL)
    {
    }

    SyntaxTree_DeclareName::SyntaxTree_DeclareName(const string& name, bool bArray, SyntaxTree_Exp* pExp)
        : parent(sizeof(SyntaxTree_DeclareName))
        , name(name)
        , bArray(bArray)
        , pExp(pExp)
    {
    }

    SyntaxTree_DeclareName::~SyntaxTree_DeclareName()
    {
    }

    void SyntaxTree_DeclareName::print(ostream& stream, uint indent)const
    {
        stream << name;
        if (pExp)
        {
            stream << " = ";
            pExp->print(stream, indent);
        }
    }

    // declare_desc -> declare_desc "," "{Letter}" "=" exp
    // declare_desc -> type_desc "{Letter}" "=" exp
    bool Parser::reduceDeclareWithAssign(ushort i)
    {
        shifts.pop();

        SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
        construct(pDeclareName, shifts.top(), false, dynamic_cast<SyntaxTree_Exp*>(syntaxTreeStack.top()));

        context.data.insert(pDeclareName);

        syntaxTreeStack.pop();
        syntaxTreeStack.top()->pushChild(pDeclareName); // syntaxTreeStack.top()->type() == "SyntaxTree_Type"

        shifts.pop();
        if (i == DECLARE_DESC_DECLARE_DESC_TYPE_DESC_LETTER_EQUAL_EXP) shifts.pop();

        return true;
    }

    // declare_desc -> declare_desc "," "{Letter}"
    // declare_desc -> type_desc "{Letter}"
    bool Parser::reduceDeclareWithoutAssign(ushort i)
    {
        SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
        construct(pDeclareName, shifts.top(), false);

        context.data.insert(pDeclareName);

        syntaxTreeStack.top()->pushChild(pDeclareName); // syntaxTreeStack.top()->type() == "SyntaxTree_Type"

        shifts.pop();
        if (i == DECLARE_DESC_DECLARE_DESC_TYPE_DESC_LETTER) shifts.pop();

        return true;
    }
}
