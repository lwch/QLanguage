#ifndef _QLANGUAGE_SYNTAX_TREE_VALUES_H_
#define _QLANGUAGE_SYNTAX_TREE_VALUES_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_Values : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_Values(bool bTop = false);
        virtual ~SyntaxTree_Values();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_Values"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Values*, &x);
#endif
            return bTop == dynamic_cast<const SyntaxTree_Values*>(&x)->bTop && childs.size() == dynamic_cast<const SyntaxTree_Values*>(&x)->childs.size();
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_Values*, &x);
#endif
            return bTop != dynamic_cast<const SyntaxTree_Values*>(&x)->bTop || childs.size() != dynamic_cast<const SyntaxTree_Values*>(&x)->childs.size();
        }
    protected:
        bool bTop;
    };
}

#endif
