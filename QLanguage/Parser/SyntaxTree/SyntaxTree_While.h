#ifndef _QLANGUAGE_SYNTAX_TREE_WHILE_H_
#define _QLANGUAGE_SYNTAX_TREE_WHILE_H_

#include "SyntaxTree_Exp.h"
#include "SyntaxTree_Block.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_While : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_While(const SyntaxTree_Exp& exp, const SyntaxTree_Block& block);
        virtual ~SyntaxTree_While();
        
        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_While*, &x);
#endif
            return exp == dynamic_cast<const SyntaxTree_While*>(&x)->exp ||
                   block == dynamic_cast<const SyntaxTree_While*>(&x)->block;
        }
        
        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_While*, &x);
#endif
            return exp != dynamic_cast<const SyntaxTree_While*>(&x)->exp ||
                   block != dynamic_cast<const SyntaxTree_While*>(&x)->block;
        }
    protected:
        const SyntaxTree_Exp&   exp;
        const SyntaxTree_Block& block;
    };
}

#endif
