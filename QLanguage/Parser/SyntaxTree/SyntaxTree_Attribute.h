/********************************************************************
	created:	2013/06/06
	created:	6:6:2013   14:17
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Attribute.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Attribute
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_ATTRIBUTE_H_
#define _QLANGUAGE_SYNTAX_TREE_ATTRIBUTE_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Attribute : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        enum Type
        {
            Public,
            Private,
            Protected
        };
    public:
        SyntaxTree_Attribute(Type type);
        virtual ~SyntaxTree_Attribute();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Attribute"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Attribute*, &x);
#endif
            return _type == dynamic_cast<const SyntaxTree_Attribute*>(&x)->_type;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Attribute*, &x);
#endif
            return _type != dynamic_cast<const SyntaxTree_Attribute*>(&x)->_type;
        }
    protected:
        Type _type;
    };
}

#endif
