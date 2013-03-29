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
    LR0::LR0() : pStart(NULL)
    {
    }

    LR0::LR0(const vector<Production>& productions, const Production::Item& start) : start(start), inputProductions(productions), pStart(NULL)
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

        typedef pair<Item*, vector<Production::Item> > pair_type;
        vector<Production::Item> v;
        pStart = closure(kernel);
        vs(pStart, v);
        queue<pair_type> s;
        s.push(pair_type(pStart, v));

        bool bContinue = true;
        while (bContinue && !s.empty())
        {
            pair_type item = s.front();
            s.pop();
            for (vector<Production::Item>::const_iterator i = v.begin(), m = v.end(); i != m; ++i)
            {
                Item* pNewItem = go(item.first, *i);
                if (pNewItem == NULL) continue;
                v.clear();
                vs(pNewItem, v);
                edges[item.first].push_back(Edge(item.first, pNewItem, *i));
                s.push(pair_type(pNewItem, v));
            }
        }

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

        int j = pItem->data.size();

        return pItem;
    }

    void LR0::closure(const LR0Production& x, vector<LR0Production>& y)
    {
        y.push_back_unique(x);
        if (x.right[x.idx].type == LR0Production::Item::NoTerminalSymbol) // 若是非终结符
        {
            // 将这个非终结符所对应的所有产生式加入其中
            for (vector<Production>::const_iterator j = productionMap[x.right[x.idx]].begin(), n = productionMap[x.right[x.idx]].end(); j != n; ++j)
            {
                if (!y.push_back_unique(*j)) break;
            }
        }
    }

    LR0::Item* LR0::go(Item* i, const Production::Item& x)
    {
        Item* pItem = Item_Alloc::allocate();
        construct(pItem);

        for (vector<LR0Production>::iterator j = i->data.begin(), n = i->data.end(); j != n; ++j)
        {
            if (j->right[j->idx] == x)
            {
                if (j->idx + 1 < j->right.size()) closure(j->stepUp(), pItem->data);
            }
        }

        if (context.isItemExists(pItem))
        {
            destruct(pItem, has_destruct(*pItem));
            Item_Alloc::deallocate(pItem);
            return NULL;
        }
        else context.states.insert(pItem);

        int k = pItem->data.size();

        return pItem;
    }

    void LR0::vs(Item* i, vector<Production::Item>& v)
    {
        for (vector<LR0Production>::const_iterator j = i->data.begin(), n = i->data.end(); j != n; ++j)
        {
            v.push_back_unique(j->right[j->idx]);
        }
    }

    void LR0::print()
    {
        printf("-------- LR(0) Start --------\n");
        for (hashmap<Item*, vector<Edge> >::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
        {
            for (vector<Edge>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                printf("%03d -> %03d\n\n", j->pStart->idx, j->pEnd->idx);
                if (j->item.type == Production::Item::TerminalSymbol) j->item.rule.printEpsilonNFA();
                else printf("VN: %03d\n", j->item.index);
                printf("\n");
            }
        }
        printf("start: %03d\n", pStart->idx);
        printf("--------- LR(0) End ---------\n");
    }
}
