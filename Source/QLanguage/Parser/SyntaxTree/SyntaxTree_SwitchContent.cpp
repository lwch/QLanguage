/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   14:30
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_SwitchContent.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_SwitchContent
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_SwitchContent.h"

namespace QLanguage
{
    SyntaxTree_SwitchContent::SyntaxTree_SwitchContent(SyntaxTree_Exp* pExp, const SyntaxTree_StmtList& stmtList)
        : parent(sizeof(SyntaxTree_SwitchContent))
        , pExp(pExp)
        , stmtList(stmtList)
    {
    }

    SyntaxTree_SwitchContent::SyntaxTree_SwitchContent(const SyntaxTree_StmtList& stmtList)
        : parent(sizeof(SyntaxTree_SwitchContent))
        , pExp(NULL)
        , stmtList(stmtList)
    {
    }

    SyntaxTree_SwitchContent::~SyntaxTree_SwitchContent()
    {
    }

    void SyntaxTree_SwitchContent::print(ostream& stream, uint indent)const
    {
        if (pExp)
        {
            stream << "case ";
            pExp->print(stream, indent);
            stream << ":";
        }
        else stream << "default:";
        stream << endl;
        stmtList.print(stream, indent + parent::indent);
    }

    // switch_content -> "case" exp ":" stmt_list
    bool Parser::reduceSwitchContentCase()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_StmtList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_SwitchContent* pSwitchContent = allocator<SyntaxTree_SwitchContent>::allocate();
        construct(pSwitchContent, dynamic_cast<SyntaxTree_Exp*>(syntaxTreeStack[1]), dynamic_cast<const SyntaxTree_StmtList&>(*syntaxTreeStack.top()));

        context.data.insert(pSwitchContent);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pSwitchContent);

        return true;
    }

    // switch_content -> "default" ":" stmt_list
    bool Parser::reduceSwitchContentDefault()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_StmtList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_SwitchContent* pSwitchContent = allocator<SyntaxTree_SwitchContent>::allocate();
        construct(pSwitchContent, dynamic_cast<const SyntaxTree_StmtList&>(*syntaxTreeStack.top()));

        context.data.insert(pSwitchContent);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pSwitchContent);

        return true;
    }
}
