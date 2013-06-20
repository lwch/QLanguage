/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   11:15
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Interface.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Interface
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_INTERFACE_H_
#define _QLANGUAGE_SYNTAX_TREE_INTERFACE_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Interface : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Interface(const string& name, SyntaxTree_InterfaceContentList* pContentList);
        SyntaxTree_Interface(const string& name);
        virtual ~SyntaxTree_Interface();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Interface"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Interface*, &x);
#endif
            return name == dynamic_cast<const SyntaxTree_Interface*>(&x)->name;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Interface*, &x);
#endif
            return name != dynamic_cast<const SyntaxTree_Interface*>(&x)->name;
        }
    protected:
        string name;
        SyntaxTree_InterfaceContentList* pContentList;
    };
}

#endif
