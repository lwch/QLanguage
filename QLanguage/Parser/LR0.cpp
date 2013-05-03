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
#include "../../QCore/Library/hashset.h"
#include "../../QCore/Library/fstream.h"

#include "LR0.h"

namespace QLanguage
{
    LR0::LR0(const vector<Production>& productions, const Production::Item& start) : begin("begin"), start(start), inputProductions(productions), pStart(NULL)
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
        LR0Production p(begin, right);
        p.bKernel = true;
        kernel.push_back(p);

        pStart = closure(kernel);
        items.push_back(pStart);

        uint iPos = 0;
        while (iPos < items.size())
        {
            Item* pItem = items[iPos];
            for (vector<LR0Production>::iterator i = pItem->data.begin(), m = pItem->data.end(); i != m; ++i)
            {
                if (i->idx < i->right.size())
                {
                    pair<Item*, bool> pa = go(*i, i->right[i->idx]);
                    if (pa.second) items.push_back_unique(pa.first);
                    edges[pItem].push_back_unique(Edge(pItem, pa.first, i->right[i->idx]));
                    //if (i->idx == 0 && i->right[i->idx] == i->left) edges[pItem].push_back_unique(Edge(pItem, pItem, i->right[i->idx]));
                }
            }
            ++iPos;
        }

        return true;
    }

    LR0::Item* LR0::closure(const vector<LR0Production>& x)
    {
        Item* pItem = Item_Alloc::allocate();
        context.states.insert(pItem);
        construct(pItem);
        pItem->idx = Item::inc();
        
        queue<LR0Production::Item> q;
        for (vector<LR0Production>::const_iterator i = x.begin(), m = x.end(); i != m; ++i)
        {
            pItem->data.push_back_unique(*i);
            q.push_unique(i->right[i->idx]);
        }

        bool bContinue = true;
        while (bContinue && !q.empty())
        {
            const LR0Production::Item& i = q.front();
            for (vector<Production>::const_iterator j = productionMap[i].begin(), n = productionMap[i].end(); j != n; ++j)
            {
                if (!pItem->data.push_back_unique(*j))
                {
                    bContinue = false;
                    break;
                }
                if (j->right[0].type == Production::Item::NoTerminalSymbol) q.push_unique(j->right[0]);
            }
            q.pop();
        }

        return pItem;
    }

    void LR0::closure(const LR0Production& x, vector<LR0Production>& y)
    {
        y.push_back_unique(x);

        uint iPos = 0;
        while (iPos < y.size())
        {
            const LR0Production& p = y[iPos];
            if (p.idx < p.right.size() && p.right[p.idx].type == LR0Production::Item::NoTerminalSymbol) // 若是非终结符
            {
                // 将这个非终结符所对应的所有产生式加入其中
                for (vector<Production>::const_iterator j = productionMap[p.right[p.idx]].begin(), n = productionMap[p.right[p.idx]].end(); j != n; ++j)
                {
                    if (!y.push_back_unique(*j)) break;
                }
            }
            ++iPos;
        }
    }

    pair<LR0::Item*, bool> LR0::go(LR0Production& p, const Production::Item& x)
    {
        Item* pItem = Item_Alloc::allocate();
        construct(pItem);

        LR0Production p1 = p.stepUp();
        p1.bKernel = true;
        closure(p1, pItem->data);

        Item* pOldItem = NULL;

        if (context.isItemExists(pItem, pOldItem))
        {
            destruct(pItem, has_destruct(*pItem));
            Item_Alloc::deallocate(pItem);
            return pair<Item*, bool>(pOldItem, false);
        }
        else
        {
            pItem->idx = Item::inc();
            context.states.insert(pItem);
        }

        return pair<Item*, bool>(pItem, true);
    }

    void LR0::print(Library::ostream& stream)
    {
        hashset<Item*> s;
        stream << "-------- LR(0) Start --------" << endl;
#ifdef _DEBUG
        for (hashmap<Item*, vector<Edge> >::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
        {
            for (vector<Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                j->print(stream);
                s.insert(j->pFrom);
                s.insert(j->pTo);
            }
        }
        stream << string::format("start: %03d", pStart->idx) << endl << endl;
        for (hashset<Item*>::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
        {
            stream << string::format("Item: %d", (*i)->idx) << endl;
            for (vector<LR0Production>::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                k->print(stream);
            }
            stream << endl;
        }
#endif
        stream << "--------- LR(0) End ---------" << endl;
    }
}
