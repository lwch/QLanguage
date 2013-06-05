/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   16:22
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_DeclareName.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_DeclareName
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_DECLARE_NAME_H_
#define _QLANGUAGE_SYNTAX_TREE_DECLARE_NAME_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_DeclareName : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_DeclareName(const string& name, bool bArray);
        virtual ~SyntaxTree_DeclareName();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_DeclareName*, &x);
#endif
            return name == dynamic_cast<const SyntaxTree_DeclareName*>(&x)->name &&
                   bArray == dynamic_cast<const SyntaxTree_DeclareName*>(&x)->bArray;
        }
    protected:
        static inline uint inc()
        {
            static uint i = 0;
            return i++;
        }
    protected:
        string name;
        uint   idx;
        bool   bArray;
    };
}

#endif
