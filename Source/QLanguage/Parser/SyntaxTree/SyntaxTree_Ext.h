#ifndef _QLANGUAGE_SYNTAX_TREE_EXT_H_
#define _QLANGUAGE_SYNTAX_TREE_EXT_H_

#include "SyntaxTree_Exp.h"

namespace QLanguage
{
    template <typename True_Func, typename False_Func, typename NoConst_Func>
    bool make_exp(Parser* pParser, SyntaxTree_Exp& exp, True_Func tf, False_Func ff, NoConst_Func ncf)
    {
        if (!exp.make(pParser)) return false;
        if (exp.isConstValue())
        {
            if (exp.toVariant(pParser).toBool())
            {
                return tf(pParser);
            }
            else
            {
                return ff(pParser);
            }
        }
        else
        {
            return ncf(pParser);
        }
    }
}

#endif
