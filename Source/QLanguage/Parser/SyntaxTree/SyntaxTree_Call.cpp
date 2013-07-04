/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   14:04
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Call.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Call
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_ValueList.h"
#include "SyntaxTree_Call.h"

namespace QLanguage
{
    SyntaxTree_Call::SyntaxTree_Call(const SyntaxTree_MemberList& memberList)
        : parent(sizeof(SyntaxTree_Call))
        , memberList(memberList)
        , pValueList(NULL)
    {
    }

    SyntaxTree_Call::SyntaxTree_Call(const SyntaxTree_MemberList& memberList, SyntaxTree_ValueList* pValueList)
        : parent(sizeof(SyntaxTree_Call))
        , memberList(memberList)
        , pValueList(pValueList)
    {
    }

    SyntaxTree_Call::~SyntaxTree_Call()
    {
    }

    void SyntaxTree_Call::print(ostream& stream, uint indent)const
    {
        memberList.print(stream, indent);
        stream << '(';
        if (pValueList) pValueList->print(stream, indent);
        stream << ')';
    }

    // call_desc -> member_desc "(" value_list ")"
    bool Parser::reduceCall1()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ValueList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Call* pCall = allocator<SyntaxTree_Call>::allocate();
        construct(pCall, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack[1]), dynamic_cast<SyntaxTree_ValueList*>(syntaxTreeStack.top()));

        context.data.insert(pCall);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pCall);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // call_desc -> member_desc "(" ")"
    bool Parser::reduceCall2()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Call* pCall = allocator<SyntaxTree_Call>::allocate();
        construct(pCall, dynamic_cast<const SyntaxTree_MemberList&>(*syntaxTreeStack.top()));

        context.data.insert(pCall);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pCall);

        shifts.pop();
        shifts.pop();
        return true;
    }
}
