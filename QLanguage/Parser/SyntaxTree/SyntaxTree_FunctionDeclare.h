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

#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Type;
    class SyntaxTree_Block;

    class SyntaxTree_Function_Declare : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Function_Declare(const string& name, const SyntaxTree_Type& returnType);
        SyntaxTree_Function_Declare(const string& name, const SyntaxTree_Type& returnType, SyntaxTree_ParamterList* pParamterList);
        virtual ~SyntaxTree_Function_Declare();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Function_Declare*, &x);
#endif
            return name == dynamic_cast<const SyntaxTree_Function_Declare*>(&x)->name &&
                  *pParamterList == *dynamic_cast<const SyntaxTree_Function_Declare*>(&x)->pParamterList &&
                   returnType == dynamic_cast<const SyntaxTree_Function_Declare*>(&x)->returnType;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Function_Declare*, &x);
#endif
            return name != dynamic_cast<const SyntaxTree_Function_Declare*>(&x)->name ||
                  *pParamterList != *dynamic_cast<const SyntaxTree_Function_Declare*>(&x)->pParamterList ||
                   returnType != dynamic_cast<const SyntaxTree_Function_Declare*>(&x)->returnType;
        }
    protected:
        string name;
        const SyntaxTree_Type&   returnType;
        SyntaxTree_ParamterList* pParamterList;
    };
}

#endif
