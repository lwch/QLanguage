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
        LR0Production() : Production() {}
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

        void print()const
        {
            printf("%s ->", left.name.c_str());
            uint j = 0;
            for (vector<Item>::const_iterator i = right.begin(), m = right.end(); i != m; ++i,++j)
            {
                if (j == idx) printf(" .");
                if (i->type == Item::TerminalSymbol)
                {
                    printf(" ");
                    i->rule.printShowName();
                }
                else printf(" %s", i->name.c_str());
            }
            printf("\n");
        }

        void print(fstream& fs)const
        {
            fs << string::format("%s ->", left.name.c_str());
            uint j = 0;
            for (vector<Item>::const_iterator i = right.begin(), m = right.end(); i != m; ++i,++j)
            {
                if (j == idx) fs << " .";
                if (i->type == Item::TerminalSymbol)
                {
                    fs << " ";
                    i->rule.printShowName(fs);
                }
                else fs << string::format(" %s", i->name.c_str());
            }
            fs << endl;
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
        typedef LR0Production parent;
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

        LALR1Production() : LR0Production() {}
        LALR1Production(const Production::Item& left) : LR0Production(left) {}
        LALR1Production(const Production::Item& left, const vector<Production::Item>& right) : LR0Production(left, right) {}
        LALR1Production(const LALR1Production& p) : LR0Production(p), wildCards(p.wildCards) {}
        LALR1Production(const LR0Production& p) : LR0Production(p) {}

        inline const bool operator==(const LALR1Production& p)const
        {
            return wildCards == p.wildCards && static_cast<LR0Production>(*this) == static_cast<LR0Production>(p);
        }

        void print()const
        {
            parent::print();
            printf("wildCards:\n");
            for (vector<Item>::const_iterator i = wildCards.begin(), m = wildCards.end(); i != m; ++i)
            {
                if (i->type == Item::Rule)
                {
                    i->rule.printShowName();
                    printf(" ");
                }
                else printf("# ");
            }
            printf("\n");
        }

        void print(fstream& fs)const
        {
            parent::print(fs);
            fs << "wildCards:" << endl;
            for (vector<Item>::const_iterator i = wildCards.begin(), m = wildCards.end(); i != m; ++i)
            {
                if (i->type == Item::Rule)
                {
                    i->rule.printShowName(fs);
                    fs << " ";
                }
                else fs << "# ";
            }
            fs << endl;
        }
    public:
        vector<Item> wildCards;
    };
}

#endif
