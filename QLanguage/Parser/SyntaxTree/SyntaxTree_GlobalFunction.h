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

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Type;
    class SyntaxTree_Block;

    class SyntaxTree_GlobalFunction : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_GlobalFunction(const string& name, SyntaxTree_Type& returnType, SyntaxTree_Block& block);
        virtual ~SyntaxTree_GlobalFunction();

        virtual void print(ostream& stream, uint indent)const;

        const bool operator==(const SyntaxTree_GlobalFunction& x)const;
    protected:
        string                   name;
        SyntaxTree_Type&         returnType;
        vector<SyntaxTree_Type*> params;
        SyntaxTree_Block&        block;
    };
}

#endif
