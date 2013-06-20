#ifndef _QLANGUAGE_SYNTAX_TREE_H_
#define _QLANGUAGE_SYNTAX_TREE_H_

#include "SyntaxTree_Block.h"
#include "SyntaxTree_Exp.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Do : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Do(const SyntaxTree_Block& block, const SyntaxTree_Exp& exp);
        virtual ~SyntaxTree_Do();
        
        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_Do"; }
        
        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Do*, &x);
#endif
            return block == dynamic_cast<const SyntaxTree_Do*>(&x)->block &&
                   exp == dynamic_cast<const SyntaxTree_Do*>(&x)->exp;
        }
        
        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Do*, &x);
#endif
            return block != dynamic_cast<const SyntaxTree_Do*>(&x)->block ||
                   exp != dynamic_cast<const SyntaxTree_Do*>(&x)->exp;
        }
    protected:
        const SyntaxTree_Block& block;
        const SyntaxTree_Exp&   exp;
    };
}

#endif
