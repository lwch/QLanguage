/********************************************************************
	created:	2013/03/09
	created:	9:3:2013   14:26
	filename: 	\QLanguage\Parser\LR0.cpp
	file path:	\QLanguage\Parser
	file base:	LR0
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "LR0.h"

namespace QLanguage
{
    LR0::LR0()
    {
    }

    LR0::LR0(const vector<Production>& productions) : inputProductions(productions)
    {
    }

    LR0::~LR0()
    {
    }
}
