/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:11
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_For.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_For
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_FOR_H_
#define _QLANGUAGE_SYNTAX_TREE_FOR_H_

#include "SyntaxTree_Exp.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_For : public SyntaxTree_Base
    {
	typedef SyntaxTree_Base parent;
    public:
	SyntaxTree_For(const SyntaxTree_Base& stmt1, const SyntaxTree_Exp& exp, const SyntaxTree_Base& stmt2, const SyntaxTree_Block& block);
	virtual ~SyntaxTree_For();
	
	virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_For"; }
	
	inline virtual const bool operator==(const SyntaxTree_Base& x)const
	{
#ifdef _DEBUG
	    TRY_CAST(const SyntaxTree_For*, &x);
#endif
	    return stmt1 == dynamic_cast<const SyntaxTree_For*>(&x)->stmt1 &&
	           exp == dynamic_cast<const SyntaxTree_For*>(&x)->exp &&
	           stmt2 == dynamic_cast<const SyntaxTree_For*>(&x)->stmt2 &&
	           block == dynamic_cast<const SyntaxTree_For*>(&x)->block;
	}
	
	inline virtual const bool operator!=(const SyntaxTree_Base& x)const
	{
#ifdef _DEBUG
	    TRY_CAST(const SyntaxTree_For*, &x);
#endif
	    return stmt1 != dynamic_cast<const SyntaxTree_For*>(&x)->stmt1 ||
	           exp != dynamic_cast<const SyntaxTree_For*>(&x)->exp ||
	           stmt2 != dynamic_cast<const SyntaxTree_For*>(&x)->stmt2 ||
	           block != dynamic_cast<const SyntaxTree_For*>(&x)->block;
	}
    protected:
	const SyntaxTree_Base&  stmt1;
	const SyntaxTree_Exp&   exp;
	const SyntaxTree_Base&  stmt2;
	const SyntaxTree_Block& block;
    };
}

#endif
