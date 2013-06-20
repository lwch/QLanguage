/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   11:27
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Name.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Name
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "SyntaxTree_Name.h"

namespace QLanguage
{
    SyntaxTree_Name::SyntaxTree_Name(const string& name) : parent(sizeof(SyntaxTree_Name)), name(name)
    {
    }

    SyntaxTree_Name::~SyntaxTree_Name()
    {
    }

    void SyntaxTree_Name::print(ostream& stream, uint indent)const
    {
        stream << name;
    }
}
