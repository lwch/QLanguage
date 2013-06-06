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

#include "SyntaxTree_Attribute.h"
#include "SyntaxTree_ParamterList.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Type;
    class SyntaxTree_Block;

    class SyntaxTree_Function : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Function(SyntaxTree_Attribute& attribute, const string& name, SyntaxTree_Type& returnType, SyntaxTree_Block& block);
        SyntaxTree_Function(SyntaxTree_Attribute& attribute, const string& name, SyntaxTree_Type& returnType, SyntaxTree_ParamterList* pParamterList, SyntaxTree_Block& block);
        virtual ~SyntaxTree_Function();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Function*, &x);
#endif
            return attribute == dynamic_cast<const SyntaxTree_Function*>(&x)->attribute &&
                   name == dynamic_cast<const SyntaxTree_Function*>(&x)->name &&
                  *pParamterList == *dynamic_cast<const SyntaxTree_Function*>(&x)->pParamterList &&
                   returnType == dynamic_cast<const SyntaxTree_Function*>(&x)->returnType;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Function*, &x);
#endif
            return attribute != dynamic_cast<const SyntaxTree_Function*>(&x)->attribute ||
                   name != dynamic_cast<const SyntaxTree_Function*>(&x)->name ||
                  *pParamterList != *dynamic_cast<const SyntaxTree_Function*>(&x)->pParamterList ||
                   returnType != dynamic_cast<const SyntaxTree_Function*>(&x)->returnType;
        }
    protected:
        SyntaxTree_Attribute& attribute;
        string name;
        SyntaxTree_Type& returnType;
        SyntaxTree_ParamterList* pParamterList;
        SyntaxTree_Block& block;
    };
}

#endif
