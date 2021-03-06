﻿/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   17:28
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_GlobalFunction.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_GlobalFunction
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"

#include "SyntaxTree_Template.h"
#include "SyntaxTree_Type.h"
#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_GlobalFunction.h"

namespace QLanguage
{
    SyntaxTree_GlobalFunction::SyntaxTree_GlobalFunction(SyntaxTree_Template *pTemplate, const SyntaxTree_Type &returnType, const string &name, const SyntaxTree_Block &block)
        : parent(sizeof(SyntaxTree_GlobalFunction))
        , pTemplate(pTemplate)
        , returnType(returnType)
        , name(name)
        , pParamterList(NULL)
        , block(block)
    {
    }

    SyntaxTree_GlobalFunction::SyntaxTree_GlobalFunction(SyntaxTree_Template *pTemplate, const SyntaxTree_Type &returnType, const string &name, SyntaxTree_ParamterList *pParamterList, const SyntaxTree_Block &block)
        : parent(sizeof(SyntaxTree_GlobalFunction))
        , pTemplate(pTemplate)
        , returnType(returnType)
        , name(name)
        , pParamterList(pParamterList)
        , block(block)
    {
    }

    SyntaxTree_GlobalFunction::SyntaxTree_GlobalFunction(const SyntaxTree_Type& returnType, const string& name, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_GlobalFunction))
        , pTemplate(NULL)
        , returnType(returnType)
        , name(name)
        , pParamterList(NULL)
        , block(block)
    {
    }

    SyntaxTree_GlobalFunction::SyntaxTree_GlobalFunction(const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_GlobalFunction))
        , pTemplate(NULL)
        , returnType(returnType)
        , name(name)
        , pParamterList(pParamterList)
        , block(block)
    {
    }

    SyntaxTree_GlobalFunction::~SyntaxTree_GlobalFunction()
    {
    }

    void SyntaxTree_GlobalFunction::print(ostream& stream, uint indent)const
    {
        if (pTemplate)
        {
            pTemplate->print(stream, indent);
            stream << ' ';
        }
        returnType.print(stream, indent);
        stream << ' ' << name << '(';
        if (pParamterList) pParamterList->print(stream, indent);
        stream << ") ";
        block.print(stream, indent + parent::indent);
        stream << endl;
    }

    const HASH_KEY_TYPE SyntaxTree_GlobalFunction::hash()const
    {
        HASH_KEY_TYPE result = 0;
        if (pTemplate) result += pTemplate->hash();
        result += returnType.hash() << 1;
        result += ::hash<string>()(name);
        if (pParamterList) result += pParamterList->hash() << 1;
        return result;
    }

    bool SyntaxTree_GlobalFunction::make(Parser* pParser)
    {
        const HASH_KEY_TYPE h = hash();
        pParser->functions.push_back(Parser::FunctionInfo(h));
        pParser->makeContext.push(Parser::ContextInfo(Parser::ContextInfo::GlobalFunction, h, pParser->functions.back().regs, pParser->functions.back().constantTable));
        Parser::LabelInfo label(string::format("_gf_%ld_%s", h, name.c_str()), pParser->instructions.size());
        pParser->labels.push_back(label);
        Parser::LabelInfo& ref = pParser->labels.back();
        bool bResult = const_cast<SyntaxTree_Block&>(block).make(pParser);
        pParser->makeContext.pop();
        ref.endIdx = pParser->instructions.size();
        return bResult;
    }

    // global_function_desc -> template_desc type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceGlobalFunctionTemplateTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[3]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();

        return true;
    }

    // global_function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceGlobalFunctionTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();

        return true;
    }

    // global_function_desc -> template_desc type_desc "{Letter}" "(" ")" block
    bool Parser::reduceGlobalFunctionTemplateType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        shifts.pop();

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();

        return true;
    }

    // global_function_desc -> type_desc "{Letter}" "(" ")" block
    bool Parser::reduceGlobalFunctionType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();
        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();

        return true;
    }

    // global_function_desc -> template_desc "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceGlobalFunctionTemplateVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                  *pReturnType,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // global_function_desc -> "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceGlobalFunctionVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  *pReturnType,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // global_function_desc -> template_desc "void" "{Letter}" "(" ")" block
    bool Parser::reduceGlobalFunctionTemplateVoid()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[1]),
                  *pReturnType,
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // global_function_desc -> "void" "{Letter}" "(" ")" block
    bool Parser::reduceGlobalFunctionVoid()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pReturnType = allocator<SyntaxTree_Type>::allocate();
        construct(pReturnType, "void", SyntaxTree_Type::Void);

        context.data.insert(pReturnType);

        SyntaxTree_GlobalFunction* pGlobalFunction = allocator<SyntaxTree_GlobalFunction>::allocate();
        construct(pGlobalFunction,
                  *pReturnType,
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pGlobalFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pGlobalFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }
}
