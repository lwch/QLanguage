/********************************************************************
	created:	2013/07/07
	created:	7:7:2013   15:13
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Switch.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Switch
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Switch.h"

namespace QLanguage
{
    SyntaxTree_Switch::SyntaxTree_Switch(const SyntaxTree_Exp& exp, const SyntaxTree_SwitchContentList& contentList)
        : parent(sizeof(SyntaxTree_Switch))
        , exp(exp)
        , contentList(contentList)
    {
    }

    SyntaxTree_Switch::~SyntaxTree_Switch()
    {
    }

    void SyntaxTree_Switch::print(ostream& stream, uint indent)const
    {
        stream << "switch (";
        exp.print(stream, indent);
        stream << ") {" << endl;
        contentList.print(stream, indent);
        this->printIndent(stream, indent);
        stream << '}';
    }

    // switch_desc -> "switch" "(" exp ")" "{" switch_content_list "}"
    bool Parser::reduceSwitch()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_SwitchContentList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Switch* pSwitch = allocator<SyntaxTree_Switch>::allocate();
        construct(pSwitch, dynamic_cast<const SyntaxTree_Exp&>(*syntaxTreeStack[1]), dynamic_cast<const SyntaxTree_SwitchContentList&>(*syntaxTreeStack.top()));

        context.data.insert(pSwitch);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pSwitch);

        return true;
    }
}
