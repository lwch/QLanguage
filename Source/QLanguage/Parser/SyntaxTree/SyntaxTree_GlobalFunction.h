/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   17:24
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_GlobalFunction.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_GlobalFunction
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_GLOBAL_FUNCTION_H_
#define _QLANGUAGE_SYNTAX_TREE_GLOBAL_FUNCTION_H_

#include "SyntaxTree_Template.h"
#include "SyntaxTree_Type.h"
#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Block;

    class SyntaxTree_GlobalFunction : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_GlobalFunction(SyntaxTree_Template* pTemplate, const SyntaxTree_Type &returnType, const string &name, const SyntaxTree_Block &block);
        SyntaxTree_GlobalFunction(SyntaxTree_Template* pTemplate, const SyntaxTree_Type &returnType, const string &name, SyntaxTree_ParamterList *pParamterList, const SyntaxTree_Block &block);
        SyntaxTree_GlobalFunction(const SyntaxTree_Type& returnType, const string& name, const SyntaxTree_Block& block);
        SyntaxTree_GlobalFunction(const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList, const SyntaxTree_Block& block);
        virtual ~SyntaxTree_GlobalFunction();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_GlobalFunction"; }

        // hash = hash(template) + hash(return) * 2 + hash(name) + hash(parameters) * 2
        virtual const HASH_KEY_TYPE hash()const;

        virtual bool make(Parser* pParser);

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_GlobalFunction*, &x);
#endif
            return parent::checkEqual(pTemplate, dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->pTemplate) &&
                   returnType == dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->returnType &&
                   name == dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->name &&
                   parent::checkEqual(pParamterList, dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->pParamterList);
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_GlobalFunction*, &x);
#endif
            return parent::checkNotEqual(pTemplate, dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->pTemplate) ||
                   returnType != dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->returnType ||
                   name != dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->name ||
                   parent::checkNotEqual(pParamterList, dynamic_cast<const SyntaxTree_GlobalFunction*>(&x)->pParamterList);
        }
    protected:
        SyntaxTree_Template*     pTemplate;
        const SyntaxTree_Type&   returnType;
        string                   name;
        SyntaxTree_ParamterList* pParamterList;
        const SyntaxTree_Block&  block;
    };
}

#endif
