/********************************************************************
	created:	2013/03/09
	created:	9:3:2013   14:26
	filename: 	\QLanguage\Parser\LR0.h
	file path:	\QLanguage\Parser
	file base:	LR0
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LR0_H_
#define _QLANGUAGE_LR0_H_

#include "../../QCore/Library/vector.h"

#include "LRProduction.h"

using namespace QLanguage::Library;

namespace QLanguage
{
    class LR0
    {
    public:
        LR0();
        LR0(const vector<Production>& productions);
    protected:
        vector<Production> inputProductions;
        vector<LR0Production*> productions;
    };
}

#endif
