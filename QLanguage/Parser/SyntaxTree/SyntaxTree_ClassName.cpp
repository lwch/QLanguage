/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   13:50
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ClassName.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ClassName
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ClassName.h"

namespace QLanguage
{
    SyntaxTree_ClassName::SyntaxTree_ClassName(const string& name) : parent(sizeof(SyntaxTree_ClassName)), name(name)
    {
    }

    SyntaxTree_ClassName::SyntaxTree_ClassName() : parent(sizeof(SyntaxTree_ClassName))
    {
    }

    SyntaxTree_ClassName::~SyntaxTree_ClassName()
    {
    }

    void SyntaxTree_ClassName::print(ostream& stream, uint indent)const
    {
        stream << "class " << name;
    }

    // class_desc1 -> "class" "{Letter}"
    bool Parser::reduceClassWithName()
    {
        SyntaxTree_ClassName* pClassName = allocator<SyntaxTree_ClassName>::allocate();
        construct(pClassName, shifts.top());

        context.data.insert(pClassName);

        syntaxTreeStack.push(pClassName);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // class_desc -> "class"
    bool Parser::reduceClassWithoutName()
    {
        SyntaxTree_ClassName* pClassName = allocator<SyntaxTree_ClassName>::allocate();
        construct(pClassName);

        context.data.insert(pClassName);

        syntaxTreeStack.push(pClassName);

        shifts.pop();
        return true;
    }
}
