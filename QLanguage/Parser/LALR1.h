/********************************************************************
	created:	2013/04/01
	created:	1:4:2013   17:08
	filename: 	\QLanguage\Parser\LALR1.h
	file path:	\QLanguage\Parser
	file base:	LALR1
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LALR1_H_
#define _QLANGUAGE_LALR1_H_

#include "LR0.h"
#include "LRProduction.h"

namespace QLanguage
{
    class LALR1
    {
    public:
        LALR1(LR0& lr0);

        bool make();
    protected:
        LR0& lr0;
    };
}

#endif
