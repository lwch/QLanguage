/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   16:31
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Function.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Function
	file ext:	cpp
	author:		lwch
	
	purpose:	若不写attribute则默认为private的
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_Function.h"

namespace QLanguage
{
    SyntaxTree_Attribute::Type SyntaxTree_Function::defaultAttribute = SyntaxTree_Attribute::Private;

    SyntaxTree_Function::SyntaxTree_Function(SyntaxTree_Template* pTemplate, const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , pTemplate(pTemplate)
        , attribute(attribute)
        , returnType(returnType)
        , name(name)
        , pParamterList(pParamterList)
        , block(block)
    {
    }

    SyntaxTree_Function::SyntaxTree_Function(SyntaxTree_Template* pTemplate, const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , pTemplate(pTemplate)
        , attribute(attribute)
        , returnType(returnType)
        , name(name)
        , pParamterList(NULL)
        , block(block)
    {
    }

    SyntaxTree_Function::SyntaxTree_Function(const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , pTemplate(NULL)
        , attribute(attribute)
        , returnType(returnType)
        , name(name)
        , pParamterList(NULL)
        , block(block)
    {
    }

    SyntaxTree_Function::SyntaxTree_Function(const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList, const SyntaxTree_Block& block)
        : parent(sizeof(SyntaxTree_Function))
        , pTemplate(NULL)
        , attribute(attribute)
        , returnType(returnType)
        , name(name)
        , pParamterList(pParamterList)
        , block(block)
    {
    }

    SyntaxTree_Function::~SyntaxTree_Function()
    {
    }

    void SyntaxTree_Function::print(ostream& stream, uint indent)const
    {
        if (pTemplate)
        {
            pTemplate->print(stream, indent);
            stream << ' ';
        }
        attribute.print(stream, indent);
        stream << ' ';
        returnType.print(stream, indent);
        stream << " " << name << '(';
        if (pParamterList) pParamterList->print(stream, indent);
        stream << ")" << endl;
        block.print(stream, indent + parent::indent);
        stream << endl;
    }

    // function_desc -> template_desc attribute type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionTemplateAttributeTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[4]);
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[4]),
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[3]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }

    // function_desc -> attribute type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionAttributeTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[3]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }

    // function_desc -> template_desc type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionTemplateTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[3]),
                 *pAttribute,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }

    // function_desc -> type_desc "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionTypeParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                 *pAttribute,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[2]),
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }

    // function_desc -> template_desc attribute type_desc "{Letter}" "(" ")" block
    bool Parser::reduceFunctionTemplateAttributeType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[3]),
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[2]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }

    // function_desc -> attribute type_desc "{Letter}" "(" ")" block
    bool Parser::reduceFunctionAttributeType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[2]),
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }

    // function_desc -> template_desc type_desc "{Letter}" "(" ")" block
    bool Parser::reduceFunctionTemplateType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                 *pAttribute,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }
    
    // function_desc -> type_desc "{Letter}" "(" ")" block
    bool Parser::reduceFunctionType()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Type*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                 *pAttribute,
                  dynamic_cast<const SyntaxTree_Type&>(*syntaxTreeStack[1]),
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();

        return true;
    }

    // function_desc -> template_desc attribute "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionTemplateAttributeVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[3]);
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[3]),
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[2]),
                 *pType,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_desc -> attribute "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionAttributeVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[2]),
                 *pType,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_desc -> template_desc "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionTemplateVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                 *pAttribute,
                 *pType,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_desc -> "void" "{Letter}" "(" paramter_list ")" block
    bool Parser::reduceFunctionVoidParamters()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_ParamterList*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  *pAttribute,
                  *pType,
                  shifts.top(),
                  dynamic_cast<SyntaxTree_ParamterList*>(syntaxTreeStack[1]),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_desc -> template_desc attribute "void" "{Letter}" "(" ")" block
    bool Parser::reduceFunctionTemplateAttributeVoid()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[2]);
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[2]),
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[1]),
                 *pType,
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_desc -> attribute "void" "{Letter}" "(" ")" block
    bool Parser::reduceFunctionAttributeVoid()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Attribute*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<const SyntaxTree_Attribute&>(*syntaxTreeStack[1]),
                  *pType,
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_desc -> template_desc "void" "{Letter}" "(" ")" block
    bool Parser::reduceFunctionTemplateVoid()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Template*, syntaxTreeStack[1]);
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  dynamic_cast<SyntaxTree_Template*>(syntaxTreeStack[1]),
                 *pAttribute,
                 *pType,
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // function_desc -> "void" "{Letter}" "(" ")" block
    bool Parser::reduceFunctionVoid()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_Block*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Attribute* pAttribute = allocator<SyntaxTree_Attribute>::allocate();
        construct(pAttribute, SyntaxTree_Function::defaultAttribute);

        context.data.insert(pAttribute);

        SyntaxTree_Type* pType = allocator<SyntaxTree_Type>::allocate();
        construct(pType, "void", SyntaxTree_Type::Void);

        context.data.insert(pType);

        SyntaxTree_Function* pFunction = allocator<SyntaxTree_Function>::allocate();
        construct(pFunction,
                  *pAttribute,
                  *pType,
                  shifts.top(),
                  dynamic_cast<const SyntaxTree_Block&>(*syntaxTreeStack.top()));

        context.data.insert(pFunction);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pFunction);

        shifts.pop();
        shifts.pop();

        return true;
    }
}
