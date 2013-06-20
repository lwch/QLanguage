/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   13:46
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ClassName.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ClassName
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_CLASS_NAME_H_
#define _QLANGUAGE_SYNTAX_TREE_CLASS_NAME_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ClassName : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ClassName(const string& name);
        SyntaxTree_ClassName();
        virtual ~SyntaxTree_ClassName();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_ClassName"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassName*, &x);
#endif
            return name == dynamic_cast<const SyntaxTree_ClassName*>(&x)->name;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ClassName*, &x);
#endif
            return name != dynamic_cast<const SyntaxTree_ClassName*>(&x)->name;
        }
    protected:
        string name;
    };
}

#endif
