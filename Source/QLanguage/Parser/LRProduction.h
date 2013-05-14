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
    class LALR1Production : public Production
    {
        typedef Production parent;
    public:
        class Item
        {
        public:
            enum { Rule, End }type;
            regex::Rule rule;

            Item() : type(End) {}
            Item(const regex::Rule& rule) : type(Rule), rule(rule) {}

            inline const bool operator==(const Item& x)const
            {
                return type == x.type && (type == End ? true : rule == x.rule);
            }

            inline const bool operator==(const Production::Item& x)const
            {
                return type == End ? false : rule == x.rule;
            }

            inline const bool operator!=(const Item& x)const
            {
                return type != x.type || (type == End ? true : rule != x.rule);
            }

            Item& operator=(const Item& x)
            {
                if (&x == this) return *this;

                type = x.type;
                if (type == Rule) rule = x.rule;
                return *this;
            }
        };

        LALR1Production() : parent(), idx(0) {}
        LALR1Production(const Production::Item& left, const vector<Production::Item>& right) : parent(left, right), idx(0) {}
        LALR1Production(const Production::Item& left, const Production::Item& right, size_t pos) : parent(left, right), idx(pos) {}
        LALR1Production(const LALR1Production& p) : parent(p), idx(p.idx), wildCards(p.wildCards) {}
        LALR1Production(const LALR1Production& p, uint pos) : parent(p), idx(pos), wildCards(p.wildCards) {}
        LALR1Production(const Production& p, uint pos) : parent(p), idx(pos) {}

        const bool operator==(const LALR1Production& p)const
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

        inline const bool operator!=(const LALR1Production& p)const
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

        inline LALR1Production& operator=(const LALR1Production& p)
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

        inline LALR1Production stepUp()
        {
            LALR1Production x(*this);
            ++x.idx;
            return x;
        }

        void print(Library::ostream& stream)const
        {
#if defined(_DEBUG) && DEBUG_LEVEL == 3
            stream << string::format("%s ->", left.name.c_str());
#else
            stream << "VN ->";
#endif
            uint j = 0;
            for (vector<parent::Item>::const_iterator i = right.begin(), m = right.end(); i != m; ++i,++j)
            {
                if (j == idx) stream << " .";
                if (i->type == parent::Item::TerminalSymbol)
                {
                    stream << " ";
                    i->rule.printShowName(stream);
                }
                else
                {
#if defined(_DEBUG) && DEBUG_LEVEL == 3
                    stream << string::format(" %s", i->name.c_str());
#else
                    stream << " VN";
#endif
                }
            }
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
    public:
        uint         idx;
        vector<Item> wildCards;
    };
}

#endif
