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

    bool Parser::reduceParamterNamed()
    {
        // TODO
        return true;
    }

    bool Parser::reduceParamterNoName()
    {
        // TODO
        return true;
    }
}
