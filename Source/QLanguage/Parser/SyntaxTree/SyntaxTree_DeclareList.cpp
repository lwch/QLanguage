/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   16:26
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_DeclareList.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_DeclareName
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Exp.h"
#include "SyntaxTree_DeclareName.h"
#include "SyntaxTree_DeclareList.h"

namespace QLanguage
{
    SyntaxTree_DeclareList::SyntaxTree_DeclareList(const SyntaxTree_Type& type)
        : parent(sizeof(SyntaxTree_DeclareList))
        , _type(type)
    {
    }

    SyntaxTree_DeclareList::~SyntaxTree_DeclareList()
    {
    }

    void SyntaxTree_DeclareList::print(ostream& stream, uint indent)const
    {
        _type.print(stream, indent);
        stream << ' ';
        if (childs.size())
        {
            childs[0]->print(stream, indent);
            for (size_t i = 1, m = childs.size(); i < m; ++i)
            {
                stream << ", ";
                childs[i]->print(stream, indent);
            }
        }
    }

    bool SyntaxTree_DeclareList::make(Parser* pParser)
    {
        for (vector<SyntaxTree_Base*>::iterator i = childs.begin(), m = childs.end(); i != m; ++i)
        {
            if (!(*i)->make(pParser)) return false;
        }
        return true;
    }

    // declare_desc -> declare_desc "," "{Letter}" array_lst "=" exp
    // declare_desc -> declare_desc "," "{Letter}" "=" exp
    // declare_desc -> type_desc "{Letter}" array_lst "=" exp
    // declare_desc -> type_desc "{Letter}" "=" exp
    bool Parser::reduceDeclareWithAssign(ushort i)
    {
#ifdef _DEBUG
        switch (i)
        {
        case DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST_EQUAL_EXP:
            TRY_CAST(SyntaxTree_DeclareList*, syntaxTreeStack[2]);
            TRY_CAST(SyntaxTree_ArrayLst*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
            break;
        case DECLARE_DESC_DECLARE_DESC_LETTER_EQUAL_EXP:
            TRY_CAST(SyntaxTree_DeclareList*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST_EQUAL_EXP:
            TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[2]);
            TRY_CAST(SyntaxTree_ArrayLst*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER_EQUAL_EXP:
            TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_Exp*, syntaxTreeStack.top());
            break;
        }

#endif
        shifts.pop();

        switch (i)
        {
        case DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST_EQUAL_EXP:
            {
                SyntaxTree_DeclareList* pDeclareList = dynamic_cast<SyntaxTree_DeclareList*>(syntaxTreeStack[2]);

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top(), dynamic_cast<SyntaxTree_ArrayLst*>(syntaxTreeStack[1]), dynamic_cast<SyntaxTree_Exp*>(syntaxTreeStack.top()));

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);
                syntaxTreeStack.pop();
                syntaxTreeStack.pop();

                shifts.pop();
            }
            break;
        case DECLARE_DESC_DECLARE_DESC_LETTER_EQUAL_EXP:
            {
                SyntaxTree_DeclareList* pDeclareList = dynamic_cast<SyntaxTree_DeclareList*>(syntaxTreeStack[1]);

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top(), dynamic_cast<SyntaxTree_Exp*>(syntaxTreeStack.top()));

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);
                syntaxTreeStack.pop();

                shifts.pop();
            }
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST_EQUAL_EXP:
            {
                SyntaxTree_DeclareList* pDeclareList = allocator<SyntaxTree_DeclareList>::allocate();
                construct(pDeclareList, dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]));

                context.data.insert(pDeclareList);

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top(), dynamic_cast<SyntaxTree_ArrayLst*>(syntaxTreeStack[1]), dynamic_cast<SyntaxTree_Exp*>(syntaxTreeStack.top()));

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);
                syntaxTreeStack.pop();
                syntaxTreeStack.pop();
                syntaxTreeStack.pop();
                syntaxTreeStack.push(pDeclareList);
            }
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER_EQUAL_EXP:
            {
                SyntaxTree_DeclareList* pDeclareList = allocator<SyntaxTree_DeclareList>::allocate();
                construct(pDeclareList, dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]));

                context.data.insert(pDeclareList);

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top(), dynamic_cast<SyntaxTree_Exp*>(syntaxTreeStack.top()));

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);
                syntaxTreeStack.pop();
                syntaxTreeStack.pop();
                syntaxTreeStack.push(pDeclareList);
            }
            break;
        }

        shifts.pop();

        return true;
    }

    // declare_desc -> declare_desc "," "{letter}" array_lst
    // declare_desc -> declare_desc "," "{Letter}"
    // declare_desc -> type_desc "{Letter}" array_lst
    // declare_desc -> type_desc "{Letter}"
    bool Parser::reduceDeclareWithoutAssign(ushort i)
    {
#ifdef _DEBUG
        switch (i)
        {
        case DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST:
            TRY_CAST(SyntaxTree_DeclareList*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_ArrayLst*, syntaxTreeStack.top());
            break;
        case DECLARE_DESC_DECLARE_DESC_LETTER:
            TRY_CAST(SyntaxTree_DeclareList*, syntaxTreeStack.top());
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST:
            TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
            TRY_CAST(SyntaxTree_ArrayLst*, syntaxTreeStack.top());
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER:
            TRY_CAST(SyntaxTree_Type*, syntaxTreeStack.top());
            break;
        }
#endif
        switch (i)
        {
        case DECLARE_DESC_DECLARE_DESC_LETTER_ARRAY_LST:
            {
                SyntaxTree_DeclareList* pDeclareList = dynamic_cast<SyntaxTree_DeclareList*>(syntaxTreeStack[1]);

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top(), dynamic_cast<SyntaxTree_ArrayLst*>(syntaxTreeStack.top()));

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);
                syntaxTreeStack.pop();

                shifts.pop();
            }
            break;
        case DECLARE_DESC_DECLARE_DESC_LETTER:
            {
                SyntaxTree_DeclareList* pDeclareList = dynamic_cast<SyntaxTree_DeclareList*>(syntaxTreeStack.top());

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top());

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);

                shifts.pop();
            }
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER_ARRAY_LST:
            {
                SyntaxTree_DeclareList* pDeclareList = allocator<SyntaxTree_DeclareList>::allocate();
                construct(pDeclareList, dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]));

                context.data.insert(pDeclareList);

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top(), dynamic_cast<SyntaxTree_ArrayLst*>(syntaxTreeStack.top()));

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);
                syntaxTreeStack.pop();
                syntaxTreeStack.pop();
                syntaxTreeStack.push(pDeclareList);
            }
            break;
        case DECLARE_DESC_TYPE_DESC_LETTER:
            {
                SyntaxTree_DeclareList* pDeclareList = allocator<SyntaxTree_DeclareList>::allocate();
                construct(pDeclareList, dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack.top()));

                context.data.insert(pDeclareList);

                SyntaxTree_DeclareName* pDeclareName = allocator<SyntaxTree_DeclareName>::allocate();
                construct(pDeclareName, shifts.top());

                context.data.insert(pDeclareName);

                pDeclareList->pushChild(pDeclareName);
                syntaxTreeStack.pop();
                syntaxTreeStack.push(pDeclareList);
            }
            break;
        }

        shifts.pop();

        return true;
    }
}
