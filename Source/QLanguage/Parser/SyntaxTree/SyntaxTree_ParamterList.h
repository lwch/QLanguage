/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   15:02
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_ParamterList.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_ParamterList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_PARAMTER_LIST_H_
#define _QLANGUAGE_SYNTAX_TREE_PARAMTER_LIST_H_

#include "SyntaxTree_Base.h"

namespace QLanguage
{
    class SyntaxTree_ParamterList : public SyntaxTree_Base
    {
        typedef SyntaxTree_Base parent;
    public:
        SyntaxTree_ParamterList();
        virtual ~SyntaxTree_ParamterList();

        virtual void print(ostream& stream, uint indent)const;
        
        inline virtual string type()const { return "SyntaxTree_ParamterList"; }

        inline virtual const bool operator==(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ParamterList*, &x);
#endif
            if (childs.size() != x.childs.size()) return false;
            for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(), j = x.childs.begin(); i != m; ++i)
            {
                if (**i != **j) return false;
            }
            return true;
        }

        inline virtual const bool operator!=(const SyntaxTree_Base& x)const
        {
#ifdef _DEBUG
            TRY_CAST(const SyntaxTree_ParamterList*, &x);
#endif
            if (childs.size() != x.childs.size()) return true;
            for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(), j = x.childs.begin(); i != m; ++i)
            {
                if (**i != **j) return true;
            }
            return false;
        }
    };
}

#endif
