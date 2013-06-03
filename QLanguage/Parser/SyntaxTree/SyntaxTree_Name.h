/********************************************************************
	created:	2013/06/03
	created:	3:6:2013   11:25
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Name.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Name
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_NAME_H_
#define _QLANGUAGE_SYNTAX_TREE_NAME_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Name : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Name(const string& name);
        virtual ~SyntaxTree_Name();

        virtual void print(ostream& stream, uint indent)const;
    protected:
        string name;
    };
}

#endif
