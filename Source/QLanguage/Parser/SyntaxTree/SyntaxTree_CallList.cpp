/********************************************************************
	created:	2013/07/06
	created:	6:7:2013   17:28
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_CallList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_CallList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ValueList.h"
#include "SyntaxTree_Call.h"
#include "SyntaxTree_CallList.h"

namespace QLanguage
{
    SyntaxTree_CallList::SyntaxTree_CallList()
        : parent(sizeof(SyntaxTree_CallList))
    {
    }

    SyntaxTree_CallList::~SyntaxTree_CallList()
    {
    }

    void SyntaxTree_CallList::print(ostream& stream, uint indent)const
    {
        if (childs.size())
        {
            childs[0]->print(stream, indent);
            for (size_t i = 1, m = childs.size(); i < m; ++i)
            {
                if (childs[i])
                {
                    stream << '(';
                    childs[i]->print(stream, indent);
                    stream << ')';
                }
                else stream << "()";
            }
        }
    }

    // call_list -> call_list "(" value_list ")"
    bool Parser::reduceCallListValueList()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_CallList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ValueList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_CallList* pCallList = dynamic_cast<SyntaxTree_CallList*>(syntaxTreeStack[1]);

        pCallList->pushChild(syntaxTreeStack.top());
        syntaxTreeStack.pop();

        return true;
    }

    // call_list -> call_list "(" ")"
    bool Parser::reduceCallList()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_CallList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_CallList* pCallList = dynamic_cast<SyntaxTree_CallList*>(syntaxTreeStack.top());

        pCallList->pushChild(NULL);

        return true;
    }

    // call_list -> call_desc
    bool Parser::reduceCallListCall()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Call*, syntaxTreeStack.top());
#endif
        SyntaxTree_CallList* pCallList = allocator<SyntaxTree_CallList>::allocate();
        construct(pCallList);

        context.data.insert(pCallList);

        pCallList->pushChild(syntaxTreeStack.top());

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pCallList);

        return true;
    }
}
