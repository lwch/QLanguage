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

    LR0::LR0(const vector<Production>& productions, const Production::Item& start) : start(start), inputProductions(productions)
    {
        for (vector<Production>::const_iterator i = productions.begin(), m = productions.end(); i != m; ++i)
        {
            productionMap[i->left].push_back(*i);
        }
    }

    bool LR0::make()
    {
        vector<LR0Production> kernel;
        vector<Production::Item> right;
        right.push_back(start);
        kernel.push_back(LR0Production(Production::Item(), right));

        closure(kernel);
        return true;
    }

    LR0::Item* LR0::closure(const vector<LR0Production>& x)
    {
        Item* pItem = Item_Alloc::allocate();
        context.states.insert(pItem);
        construct(pItem);
        pItem->data.add(x);
        bool bContinue = true;
        for (vector<LR0Production>::const_iterator i = x.begin(), m = x.end(); i != m; ++i)
        {
            if (i->right[i->idx].type == LR0Production::Item::NoTerminalSymbol) // 若是非终结符
            {
                // 将这个非终结符所对应的所有产生式加入其中
                for (vector<Production>::const_iterator j = productionMap[i->right[i->idx]].begin(), n = productionMap[i->right[i->idx]].end(); j != n; ++j)
                {
                    if (!pItem->data.push_back_unique(*j))
                    {
                        bContinue = false;
                        break;
                    }
                }
            }
            if (!bContinue) break;
        }
        return pItem;
    }
}
