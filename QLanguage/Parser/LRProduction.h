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
        LR0Production(const Production::Item& left) : Production(left), idx(0) {}
        LR0Production(const Production::Item& left, const list<Production::Item>& right) : Production(left, right), idx(0) {}
        LR0Production(const LR0Production& p) : Production(p), idx(p.idx) {}
    public:
        size_t idx;
    };

    class LALR1Production : public LR0Production
    {
    public:
        LALR1Production(const Production::Item& left) : LR0Production(left) {}
        LALR1Production(const Production::Item& left, const list<Production::Item>& right) : LR0Production(left, right) {}
        LALR1Production(const LALR1Production& p) : LR0Production(p) {}
    public:
        vector<string> wildCards;
    };
}

#endif
