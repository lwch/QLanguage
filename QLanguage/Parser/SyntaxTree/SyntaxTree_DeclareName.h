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

        const bool operator==(const SyntaxTree_DeclareName& x)const;
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
