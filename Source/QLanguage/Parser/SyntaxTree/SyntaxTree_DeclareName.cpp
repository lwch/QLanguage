/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   14:40
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
    SyntaxTree_DeclareName::SyntaxTree_DeclareName(const string& name, SyntaxTree_ArrayLst* pArrayLst, SyntaxTree_Exp* pExp)
        : parent(sizeof(SyntaxTree_DeclareName))
        , name(name)
        , pArrayLst(pArrayLst)
        , pExp(pExp)
    {
    }

    SyntaxTree_DeclareName::SyntaxTree_DeclareName(const string& name, SyntaxTree_ArrayLst* pArrayLst)
        : parent(sizeof(SyntaxTree_DeclareName))
        , name(name)
        , pArrayLst(pArrayLst)
        , pExp(NULL)
    {
    }

    SyntaxTree_DeclareName::SyntaxTree_DeclareName(const string& name, SyntaxTree_Exp* pExp)
        : parent(sizeof(SyntaxTree_DeclareName))
        , name(name)
        , pArrayLst(NULL)
        , pExp(pExp)
    {
    }

    SyntaxTree_DeclareName::SyntaxTree_DeclareName(const string& name)
        : parent(sizeof(SyntaxTree_DeclareName))
        , name(name)
        , pArrayLst(NULL)
        , pExp(NULL)
    {
    }

    SyntaxTree_DeclareName::~SyntaxTree_DeclareName()
    {
    }

    void SyntaxTree_DeclareName::print(ostream& stream, uint indent)const
    {
        stream << name;
        if (pArrayLst) pArrayLst->print(stream, indent);
        if (pExp)
        {
            stream << " = ";
            pExp->print(stream, indent);
        }
    }

    const HASH_KEY_TYPE SyntaxTree_DeclareName::hash()const
    {
        return ::hash<string>()(name);
    }

    bool SyntaxTree_DeclareName::make(Parser* pParser)
    {
        bool bResult = true;
        // TODO: 分配寄存器
        if (pExp) bResult = pExp->make(pParser);
        return bResult;
    }
}
