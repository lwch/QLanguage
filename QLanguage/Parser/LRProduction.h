/********************************************************************
	created:	2013/03/10
	created:	10:3:2013   17:44
	filename: 	\QLanguage\Parser\LRProduction.h
	file path:	\QLanguage\Parser
	file base:	LRProduction
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LRPRODUCTION_H_
#define _QLANGUAGE_LRPRODUCTION_H_

#include "Production.h"

namespace QLanguage
{
    class LR0Production : public Production
    {
    public:
        LR0Production() : Production(), idx(0) {}
        LR0Production(const Production::Item& left) : Production(left), idx(0) {}
        LR0Production(const Production::Item& left, const vector<Production::Item>& right) : Production(left, right), idx(0) {}
        LR0Production(const Production& p) : Production(p), idx(0) {}
        LR0Production(const LR0Production& p) : Production(p), idx(p.idx) {}

        inline const bool operator==(const LR0Production& p)const
        {
            if (left != p.left) return false;
            if (right.size() != p.right.size()) return false;
            if (idx != p.idx) return false;

            for (vector<Production::Item>::const_iterator i = right.begin(), m = right.end(), j = p.right.begin(); i != m; ++i, ++j)
            {
                if (*i != *j) return false;
            }
            return true;
        }

        inline const bool operator!=(const LR0Production& p)const
        {
            if (left != p.left) return true;
            if (right.size() != p.right.size()) return true;
            if (idx != p.idx) return true;

            for (vector<Production::Item>::const_iterator i = right.begin(), m = right.end(), j = p.right.begin(); i != m; ++i, ++j)
            {
                if (*i != *j) return true;
            }
            return false;
        }

        inline LR0Production& operator=(const LR0Production& p)
        {
            if (&p != this)
            {
                left  = p.left;
                right = p.right;
                idx   = p.idx;
            }
            return *this;
        }

        inline LR0Production stepUp()
        {
          LR0Production x(*this);
          ++x.idx;
          return x;
        }

        inline LR0Production stepDown()
        {
          LR0Production x(*this);
          --x.idx;
          return x;
        }
    public:
        size_t idx;
    };

    class LALR1Production : public LR0Production
    {
    public:
        LALR1Production(const Production::Item& left) : LR0Production(left) {}
        LALR1Production(const Production::Item& left, const vector<Production::Item>& right) : LR0Production(left, right) {}
        LALR1Production(const LALR1Production& p) : LR0Production(p) {}
    public:
        vector<string> wildCards;
    };
}

#endif
