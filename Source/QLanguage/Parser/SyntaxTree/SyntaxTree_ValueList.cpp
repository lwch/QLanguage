/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   13:45
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ValueList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ValueList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ValueList.h"

namespace QLanguage
{
    SyntaxTree_ValueList::SyntaxTree_ValueList() : parent(sizeof(SyntaxTree_ValueList))
    {
    }

    SyntaxTree_ValueList::~SyntaxTree_ValueList()
    {
    }

    void SyntaxTree_ValueList::print(ostream& stream, uint indent)const
    {
    }
}
