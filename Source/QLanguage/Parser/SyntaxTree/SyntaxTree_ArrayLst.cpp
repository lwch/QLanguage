/********************************************************************
	created:	2013/06/29
	created:	29:6:2013   12:29
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ArrayLst.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ArrayLst
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Exp.h"
#include "SyntaxTree_ArrayLst.h"

namespace QLanguage
{
    SyntaxTree_ArrayLst::SyntaxTree_ArrayLst()
        : parent(sizeof(SyntaxTree_ArrayLst))
    {
    }

    SyntaxTree_ArrayLst::~SyntaxTree_ArrayLst()
    {
    }

    void SyntaxTree_ArrayLst::print(ostream& stream, uint indent)const
    {
        for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(); i != m; ++i)
        {
            stream << '[';
            if (*i) (*i)->print(stream, indent);
            stream << ']';
        }
    }

    // array_lst -> array_lst "[" exp "]"
    // array_lst -> array_lst "[" "]"
    bool Parser::reduceArrayLst2Size(ushort i)
    {
        if (i == ARRAY_LST_ARRAY_LST_EXP)
        {
#ifdef _DEBUG
            TRY_CAST(SyntaxTree_ArrayLst*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
        }
        else
        {
#ifdef _DEBUG
            TRY_CAST(SyntaxTree_ArrayLst*, syntaxTreeStack.top());
#endif
        }

        shifts.pop();
        shifts.pop();

        if (i == ARRAY_LST_ARRAY_LST_EXP)
        {
            SyntaxTree_ArrayLst* pArrayLst = dynamic_cast<SyntaxTree_ArrayLst*>(syntaxTreeStack[1]);

            pArrayLst->pushChild(syntaxTreeStack.top());
            syntaxTreeStack.pop();
        }
        else
        {
            SyntaxTree_ArrayLst* pArrayLst = dynamic_cast<SyntaxTree_ArrayLst*>(syntaxTreeStack.top());

            pArrayLst->pushChild(NULL);
        }

        return true;
    }

    // array_lst -> "[" exp "]"
    // array_lst -> "[" "]"
    bool Parser::reduceArrayLst1Size(ushort i)
    {
        if (i == ARRAY_LST_EXP)
        {
#ifdef _DEBUG
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
#endif
        }

        shifts.pop();
        shifts.pop();

        SyntaxTree_ArrayLst* pArrayLst = allocator<SyntaxTree_ArrayLst>::allocate();
        construct(pArrayLst);

        context.data.insert(pArrayLst);

        if (i == ARRAY_LST_EXP)
        {
            pArrayLst->pushChild(syntaxTreeStack.top());
            syntaxTreeStack.pop();
        }
        else pArrayLst->pushChild(NULL);

        syntaxTreeStack.push(pArrayLst);

        return true;
    }
}
