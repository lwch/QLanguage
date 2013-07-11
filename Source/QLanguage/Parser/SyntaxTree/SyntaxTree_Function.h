/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   16:23
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Function.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Function
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_FUNCTION_H_
#define _QLANGUAGE_SYNTAX_TREE_FUNCTION_H_

#include "SyntaxTree_Template.h"
#include "SyntaxTree_Attribute.h"
#include "SyntaxTree_Type.h"
#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Block;

    class SyntaxTree_Function : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        static SyntaxTree_Attribute::Type defaultAttribute;
    public:
        SyntaxTree_Function(SyntaxTree_Template* pTemplate, const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, const SyntaxTree_Block& block);
        SyntaxTree_Function(SyntaxTree_Template* pTemplate, const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList, const SyntaxTree_Block& block);
        SyntaxTree_Function(const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, const SyntaxTree_Block& block);
        SyntaxTree_Function(const SyntaxTree_Attribute& attribute, const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList, const SyntaxTree_Block& block);
        virtual ~SyntaxTree_Function();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Function"; }

        // hash = hash(template) + hash(attribute) * 2 + hash(returnType) * 2 + hash(name) + hash(parameters) * 2
        virtual const HASH_KEY_TYPE hash()const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Function*, &x);
#endif
            return parent::checkEqual(pTemplate, dynamic_cast<const SyntaxTree_Function*>(&x)->pTemplate) &&
                   attribute == dynamic_cast<const SyntaxTree_Function*>(&x)->attribute &&
                   name == dynamic_cast<const SyntaxTree_Function*>(&x)->name &&
                   parent::checkEqual(pParamterList, dynamic_cast<const SyntaxTree_Function*>(&x)->pParamterList) &&
                   returnType == dynamic_cast<const SyntaxTree_Function*>(&x)->returnType;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Function*, &x);
#endif
            return parent::checkNotEqual(pTemplate, dynamic_cast<const SyntaxTree_Function*>(&x)->pTemplate) ||
                   attribute != dynamic_cast<const SyntaxTree_Function*>(&x)->attribute ||
                   name != dynamic_cast<const SyntaxTree_Function*>(&x)->name ||
                   parent::checkNotEqual(pParamterList, dynamic_cast<const SyntaxTree_Function*>(&x)->pParamterList) ||
                   returnType != dynamic_cast<const SyntaxTree_Function*>(&x)->returnType;
        }
    protected:
        SyntaxTree_Template*        pTemplate;
        const SyntaxTree_Attribute& attribute;
        const SyntaxTree_Type&      returnType;
        string                      name;
        SyntaxTree_ParamterList*    pParamterList;
        const SyntaxTree_Block&     block;
    };
}

#endif
