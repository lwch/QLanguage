/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   16:38
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_FunctionDeclare.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_FunctionDeclare
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_FUNCTION_DECLARE_H_
#define _QLANGUAGE_SYNTAX_TREE_FUNCTION_DECLARE_H_

#include "SyntaxTree_Template.h"
#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Type;
    class SyntaxTree_Block;

    class SyntaxTree_FunctionDeclare : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_FunctionDeclare(SyntaxTree_Template* pTemplate, const SyntaxTree_Type& returnType, const string& name);
        SyntaxTree_FunctionDeclare(SyntaxTree_Template* pTemplate, const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList);
        SyntaxTree_FunctionDeclare(const SyntaxTree_Type& returnType, const string& name);
        SyntaxTree_FunctionDeclare(const SyntaxTree_Type& returnType, const string& name, SyntaxTree_ParamterList* pParamterList);
        virtual ~SyntaxTree_FunctionDeclare();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Function_Declare"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_FunctionDeclare*, &x);
#endif
            return parent::checkEqual(pTemplate, dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->pTemplate) &&
                   returnType == dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->returnType &&
                   name == dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->name &&
                   parent::checkEqual(pParamterList, dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->pParamterList);
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_FunctionDeclare*, &x);
#endif
            return parent::checkNotEqual(pTemplate, dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->pTemplate) ||
                   returnType != dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->returnType ||
                   name != dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->name ||
                   parent::checkNotEqual(pParamterList, dynamic_cast<const SyntaxTree_FunctionDeclare*>(&x)->pParamterList);
        }
    protected:
        SyntaxTree_Template*     pTemplate;
        const SyntaxTree_Type&   returnType;
        string                   name;
        SyntaxTree_ParamterList* pParamterList;
    };
}

#endif
