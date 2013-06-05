/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   15:04
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ParamterList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ParamterList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ParamterList.h"

namespace QLanguage
{
    SyntaxTree_ParamterList::SyntaxTree_ParamterList() : parent(sizeof(SyntaxTree_ParamterList))
    {
    }

    SyntaxTree_ParamterList::~SyntaxTree_ParamterList()
    {
    }

    void SyntaxTree_ParamterList::print(ostream& stream, uint indent)const
    {
    }
}
