/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   16:32
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Paramter.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Paramter
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Paramter.h"

namespace QLanguage
{
    SyntaxTree_Paramter::SyntaxTree_Paramter(const SyntaxTree_Type& type, const string& name, SyntaxTree_ArrayLst* pArrayLst)
        : parent(sizeof(SyntaxTree_Paramter))
        , _type(type)
        , name(name)
        , pArrayLst(pArrayLst)
    {
    }

    SyntaxTree_Paramter::SyntaxTree_Paramter(const SyntaxTree_Type& type, const string& name)
        : parent(sizeof(SyntaxTree_Paramter))
        , _type(type)
        , name(name)
        , pArrayLst(NULL)
    {
    }

    SyntaxTree_Paramter::SyntaxTree_Paramter(const SyntaxTree_Type& type)
        : parent(sizeof(SyntaxTree_Paramter))
        , _type(type)
        , pArrayLst(NULL)
    {
    }

    SyntaxTree_Paramter::~SyntaxTree_Paramter()
    {
    }

    void SyntaxTree_Paramter::print(ostream& stream, uint indent)const
    {
        _type.print(stream, indent);
        if (name != "")
        {
            stream << ' ' << name;
            if (pArrayLst) pArrayLst->print(stream, indent);
        }
    }

    const HASH_KEY_TYPE SyntaxTree_Paramter::hash()const
    {
        HASH_KEY_TYPE result = _type.hash();
        if (pArrayLst) result += pArrayLst->size();
        return result;
    }

    // paramter -> type_desc "{Letter}" array_lst
    bool Parser::reduceParamterNamedArray()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ArrayLst*, syntaxTreeStack.top());
#endif
        SyntaxTree_Paramter* pParamter = allocator<SyntaxTree_Paramter>::allocate();
        construct(pParamter, dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]), shifts.top(), dynamic_cast<SyntaxTree_ArrayLst*>(syntaxTreeStack.top()));

        context.data.insert(pParamter);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pParamter);

        shifts.pop();

        return true;
    }

    // paramter -> type_desc "{Letter}"
    bool Parser::reduceParamterNamed()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack.top());
#endif
        SyntaxTree_Paramter* pParamter = allocator<SyntaxTree_Paramter>::allocate();
        construct(pParamter, dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack.top()), shifts.top());

        context.data.insert(pParamter);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pParamter);

        shifts.pop();

        return true;
    }

    // paramter -> type_desc
    bool Parser::reduceParamterNoName()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack.top());
#endif
        SyntaxTree_Paramter* pParamter = allocator<SyntaxTree_Paramter>::allocate();
        construct(pParamter, dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack.top()));

        context.data.insert(pParamter);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pParamter);

        return true;
    }
}
