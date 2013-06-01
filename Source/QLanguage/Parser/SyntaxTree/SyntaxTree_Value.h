/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   20:00
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Value.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Value
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_VALUE_H_
#define _QLANGUAGE_SYNTAX_TREE_VALUE_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Value : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            Boolean,
            Real,
            Digit,
            String
        };
    public:
        SyntaxTree_Value(const string& value, Type type);
        virtual ~SyntaxTree_Value();

        virtual void print(ostream& stream, uint indent)const;

        const bool operator==(const SyntaxTree_Value& x)const;
    protected:
        string value;
        Type   type;
    };
}

#endif
