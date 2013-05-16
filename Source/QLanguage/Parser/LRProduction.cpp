/********************************************************************
	created:	2013/05/16
	created:	16:5:2013   14:14
	filename: 	\QLanguage\Parser\LRProduction.cpp
	file path:	\QLanguage\Parser
	file base:	LRProduction
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "LRProduction.h"

namespace QLanguage
{
    // LALR1Production::Item
    LALR1Production::Item::Item() : type(End)
    {
    }

    LALR1Production::Item::Item(const regex::Rule& rule) : type(Rule), rule(rule)
    {
    }

    LALR1Production::Item& LALR1Production::Item::operator=(const Item& x)
    {
        if (&x == this) return *this;

        type = x.type;
        if (type == Rule) rule = x.rule;
        return *this;
    }
    // LALR1Production
    LALR1Production::LALR1Production() : parent(), idx(0)
    {
    }

    LALR1Production::LALR1Production(const Production::Item& left, const vector<Production::Item>& right) : parent(left, right), idx(0)
    {
    }

    LALR1Production::LALR1Production(const Production::Item& left, const Production::Item& right, size_t pos) : parent(left, right), idx(pos)
    {
    }

    LALR1Production::LALR1Production(const Production& p, uint pos) : parent(p), idx(pos)
    {
    }

    LALR1Production::LALR1Production(const LALR1Production& p) : parent(p), idx(p.idx), wildCards(p.wildCards)
    {
    }

    LALR1Production::LALR1Production(const LALR1Production& p, uint pos) : parent(p), idx(pos), wildCards(p.wildCards)
    {
    }

    const bool LALR1Production::operator==(const LALR1Production& p)const
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

    const bool LALR1Production::operator!=(const LALR1Production& p)const
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

    LALR1Production& LALR1Production::operator=(const LALR1Production& p)
    {
        if (&p != this)
        {
            left      = p.left;
            right     = p.right;
            idx       = p.idx;
            wildCards = p.wildCards;
        }
        return *this;
    }

    void LALR1Production::print(Library::ostream& stream)const
    {
        parent::print(stream);
        stream << "wildCards:" << endl;
        for (vector<Item>::const_iterator i = wildCards.begin(), m = wildCards.end(); i != m; ++i)
        {
            if (i->type == Item::Rule)
            {
                i->rule.printShowName(stream);
                stream << " ";
            }
            else stream << "# ";
        }
        stream << endl;
    }
}