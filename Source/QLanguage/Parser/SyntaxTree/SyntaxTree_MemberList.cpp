﻿/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   11:19
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_MemberList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_MemberList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Name.h"
#include "SyntaxTree_MemberList.h"

namespace QLanguage
{
    SyntaxTree_MemberList::SyntaxTree_MemberList() : parent(sizeof(SyntaxTree_MemberList))
    {
    }

    SyntaxTree_MemberList::~SyntaxTree_MemberList()
    {
    }

    void SyntaxTree_MemberList::print(ostream& stream, uint indent)const
    {
        childs[0]->print(stream, indent);
        for (size_t i = 1, m = childs.size(); i < m; ++i)
        {
            stream << '.';
            childs[i]->print(stream, indent);
        }
    }

    const string SyntaxTree_MemberList::name()const
    {
        if (childs.size() == 1) return dynamic_cast<SyntaxTree_Name*>(childs[0])->name;
        string result = dynamic_cast<SyntaxTree_Name*>(childs[0])->name;
        for (size_t i = 1, m = childs.size(); i < m; ++i)
        {
            result += "." + dynamic_cast<SyntaxTree_Name*>(childs[i])->name;
        }
        return result;
    }

    const HASH_KEY_TYPE SyntaxTree_MemberList::hash()const
    {
        HASH_KEY_TYPE result = 0;
        for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(); i != m; ++i)
        {
            result += (*i)->hash();
        }
        return result;
    }

    // member_desc -> member_desc "." "{Letter}"
    bool Parser::reduceMember2Size()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_MemberList*, syntaxTreeStack.top());
#endif
        SyntaxTree_Name* pName = allocator<SyntaxTree_Name>::allocate();
        construct(pName, shifts.top());

        context.data.insert(pName);

        SyntaxTree_MemberList* pMemberList = dynamic_cast<SyntaxTree_MemberList*>(syntaxTreeStack.top());
        pMemberList->pushChild(pName);

        shifts.pop();
        shifts.pop();
        return true;
    }

    // member_desc -> "{Letter}"
    bool Parser::reduceMember1Size()
    {
        SyntaxTree_Name* pName = allocator<SyntaxTree_Name>::allocate();
        construct(pName, shifts.top());

        context.data.insert(pName);

        SyntaxTree_MemberList* pMemberList = allocator<SyntaxTree_MemberList>::allocate();
        construct(pMemberList);

        context.data.insert(pMemberList);

        pMemberList->pushChild(pName);

        syntaxTreeStack.push(pMemberList);

        shifts.pop();
        return true;
    }
}
