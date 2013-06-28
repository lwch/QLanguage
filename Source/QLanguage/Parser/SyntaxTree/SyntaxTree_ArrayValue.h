#ifndef _QLANGUAGE_SYNTAXTREE_ARRAYVALUE_H_
#define _QLANGUAGE_SYNTAXTREE_ARRAYVALUE_H_

#include "SyntaxTree_Values.h"
#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ArrayValue : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ArrayValue(SyntaxTree_Values* pValues);
        virtual ~SyntaxTree_ArrayValue();

        virtual void print(ostream& stream, uint indent)const;

        inline virtual string type()const { return "SyntaxTree_ArrayValue"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ArrayValue*, &x);
#endif
            return true;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ArrayValue*, &x);
#endif
            return true;
        }
    protected:
        SyntaxTree_Values* pValues;
    };
}

#endif
