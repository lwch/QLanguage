/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   16:31
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Function.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Function
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Function.h"

namespace QLanguage
{
    SyntaxTree_Function::SyntaxTree_Function(SyntaxTree_Attribute& attribute, const string& name, SyntaxTree_Type& returnType, SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , attribute(attribute)
        , name(name)
        , returnType(returnType)
        , block(block)
    {
    }

    SyntaxTree_Function::SyntaxTree_Function(SyntaxTree_Attribute& attribute, const string& name, SyntaxTree_Type& returnType, SyntaxTree_ParamterList* pParamterList, SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , attribute(attribute)
        , name(name)
        , returnType(returnType)
        , pParamterList(pParamterList)
        , block(block)
    {
    }

    SyntaxTree_Function::~SyntaxTree_Function()
    {
    }

    void SyntaxTree_Function::print(ostream& stream, uint indent)const
    {
    }
}
