/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   14:44
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ClassInherit.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ClassInherit
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ClassInherit.h"

namespace QLanguage
{
    SyntaxTree_ClassInherit::SyntaxTree_ClassInherit(const SyntaxTree_Attribute& attribute, Type type, const string& name)
        : parent(sizeof(SyntaxTree_ClassInherit))
        , attribute(attribute)
        , _type(type)
        , name(name)
    {
    }

    SyntaxTree_ClassInherit::~SyntaxTree_ClassInherit()
    {
    }

    void SyntaxTree_ClassInherit::print(ostream& stream, uint indent)const
    {
        attribute.print(stream, indent);
        stream << ' ';
        switch (_type)
        {
        case Implement:
            stream << "implement ";
            break;
        case Extend:
            stream << "extend ";
            break;
        }
        stream << name;
    }

    // class_desc2 -> attribute "implement" "{Letter}"
    bool Parser::reduceClassImplement()
    {
        SyntaxTree_ClassInherit* pClassInherit = allocator<SyntaxTree_ClassInherit>::allocate();
        construct(pClassInherit, dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack.top()), SyntaxTree_ClassInherit::Implement, shifts.top());

        context.data.insert(pClassInherit);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClassInherit);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // class_desc2 -> attribute "extend" "{Letter}"
    bool Parser::reduceClassExtend()
    {
        SyntaxTree_ClassInherit* pClassInherit = allocator<SyntaxTree_ClassInherit>::allocate();
        construct(pClassInherit, dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack.top()), SyntaxTree_ClassInherit::Extend, shifts.top());

        context.data.insert(pClassInherit);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pClassInherit);

        shifts.pop();
        shifts.pop();
        return true;
    }
}
