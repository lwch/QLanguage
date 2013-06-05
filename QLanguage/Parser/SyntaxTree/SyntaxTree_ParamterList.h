/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   15:02
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ParamterList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ParamterList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_PARAMTER_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_PARAMTER_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ParamterList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ParamterList();
        virtual ~SyntaxTree_ParamterList();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_ParamterList& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ParamterList*, &x);
#endif
            // TODO 实现类型的比较
            return true;
        }
    };
}

#endif
