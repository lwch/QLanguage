/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   16:41
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_FunctionDeclare.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_FunctionDeclare
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"

#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_FunctionDeclare.h"

namespace QLanguage
{
    SyntaxTree_FunctionDeclare::SyntaxTree_FunctionDeclare(SyntaxTree_Template* pTemplate, const SyntaxTree_Type& returnType, const string& name)
        : parent(sizeof(SyntaxTree_FunctionDeclare))
        , pTemplate(pTemplate)
        , returnType(returnType)
        , name(name)
        , pParamterList(NULL)
    {
    }

    SyntaxTree_FunctionDeclare::SyntaxTree_FunctionDeclare(SyntaxTree_Template* pTemplate, const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList)
        : parent(sizeof(SyntaxTree_FunctionDeclare))
        , pTemplate(pTemplate)
        , returnType(returnType)
        , name(name)
        , pParamterList(pParamterList)
    {
    }

    SyntaxTree_FunctionDeclare::SyntaxTree_FunctionDeclare(const SyntaxTree_Type& returnType, const string& name)
        : parent(sizeof(SyntaxTree_FunctionDeclare))
        , pTemplate(NULL)
        , returnType(returnType)
        , name(name)
        , pParamterList(NULL)
    {
    }

    SyntaxTree_FunctionDeclare::SyntaxTree_FunctionDeclare(const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList)
        : parent(sizeof(SyntaxTree_FunctionDeclare))
        , pTemplate(NULL)
        , returnType(returnType)
        , name(name)
        , pParamterList(pParamterList)
    {
    }

    SyntaxTree_FunctionDeclare::~SyntaxTree_FunctionDeclare()
    {
    }

    void SyntaxTree_FunctionDeclare::print(ostream& stream, uint indent)const
    {
        if (pTemplate)
        {
            pTemplate->print(stream, indent);
            stream << ' ';
        }
        returnType.print(stream, indent);
        stream << ' ' << name << '(';
        if (pParamterList) pParamterList->print(stream, indent);
        stream << ");";
    }

    const HASH_KEY_TYPE SyntaxTree_FunctionDeclare::hash()const
    {
        HASH_KEY_TYPE result = 0;
        if (pTemplate) result += pTemplate->hash();
        result += returnType.hash() << 1;
        result += ::hash<string>()(name);
        if (pParamterList) result += pParamterList->hash() << 1;
        return result;
    }

    // function_declare -> template_desc type_desc "{Letter}" "(" paramter_list ")" ";"
    bool Parser::reduceFunctionDeclareTemplateTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack.top()));

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();

        return true;
    }

    // function_declare -> type_desc "{Letter}" "(" paramter_list ")" ";"
    bool Parser::reduceFunctionDeclareTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack.top()));

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();

        return true;
    }

    // function_declare -> template_desc type_desc "{Letter}" "(" ")" ";"
    bool Parser::reduceFunctionDeclareTemplateType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack.top()),
                  shifts.top());

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();

        return true;
    }

    // function_declare -> type_desc "{Letter}" "(" ")" ";"
    bool Parser::reduceFunctionDeclareType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack.top()),
                  shifts.top());

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();

        return true;
    }

    // function_declare -> template_desc "void" "{Letter}" "(" paramter_list ")" ";"
    bool Parser::reduceFunctionDeclareTemplateVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare, dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[1]), *pReturnType, shifts.top());

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_declare -> "void" "{Letter}" "(" paramter_list ")" ";"
    bool Parser::reduceFunctionDeclareVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare, *pReturnType, shifts.top());

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_declare -> template_desc "void" "{Letter}" "(" ")" ";"
    bool Parser::reduceFunctionDeclareTemplateVoid()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare, dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[1]), *pReturnType, shifts.top());

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_declare -> "void" "{Letter}" "(" ")" ";"
    bool Parser::reduceFunctionDeclareVoid()
    {
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_FunctionDeclare* pFunctionDeclare = allocator<SyntaxTree_FunctionDeclare>::allocate();
        construct(pFunctionDeclare, *pReturnType, shifts.top());

        context.data.insert(pFunctionDeclare);

        syntaxTreeStack.push(pFunctionDeclare);

        shifts.pop();
        shifts.pop();

        return true;
    }
}
