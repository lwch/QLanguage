/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   16:32
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Paramter.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Paramter
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Paramter.h"

namespace QLanguage
{
    SyntaxTree_Paramter::SyntaxTree_Paramter(SyntaxTree_Type& type, const string& name) : parent(sizeof(SyntaxTree_Paramter)), type(type), name(name)
    {
    }

    SyntaxTree_Paramter::SyntaxTree_Paramter(SyntaxTree_Type& type) : parent(sizeof(SyntaxTree_Paramter)), type(type)
    {
    }

    SyntaxTree_Paramter::~SyntaxTree_Paramter()
    {
    }

    void SyntaxTree_Paramter::print(ostream& stream, uint indent)const
    {
    }

    // paramter -> type_desc "{Letter}"
    bool Parser::reduceParamterNamed()
    {
        SyntaxTree_Paramter* pParamter = allocator<SyntaxTree_Paramter>::allocate();
        construct(pParamter, dynamic_cast<SyntaxTree_Type&>(*syntaxTreeStack.top()), shifts.top());

        context.data.insert(pParamter);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pParamter);

        shifts.pop();
        return true;
    }

    // paramter -> type_desc
    bool Parser::reduceParamterNoName()
    {
        SyntaxTree_Paramter* pParamter = allocator<SyntaxTree_Paramter>::allocate();
        construct(pParamter, dynamic_cast<SyntaxTree_Type&>(*syntaxTreeStack.top()));

        context.data.insert(pParamter);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pParamter);

        return true;
    }
}
