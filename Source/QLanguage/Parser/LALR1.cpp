/********************************************************************
	created:	2013/04/01
	created:	1:4:2013   17:10
	filename: 	\QLanguage\Parser\LALR1.cpp
	file path:	\QLanguage\Parser
	file base:	LALR1
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "LALR1.h"

namespace QLanguage
{
    LALR1::LALR1(const vector<Production>& productions, const Production::Item& start)
        :
#ifdef _DEBUG
        begin("begin"),
#endif
        start(start)
    {
        LALR1Production p(begin, start, 0);
        p.wildCards.push_back(LALR1Production::Item());
        inputProductions[begin].push_back(p);
        inputProductions[begin].push_back(LALR1Production(begin, start, 1));
        for (vector<Production>::const_iterator i = productions.begin(), m = productions.end(); i != m; ++i)
        {
            for (size_t j = 0, n = i->right.size(); j < n; ++j)
            {
                inputProductions[i->left].push_back(LALR1Production(*i, j));
            }
        }
    }

    bool LALR1::make()
    {
        vector<LALR1Production> v;
        v.push_back(inputProductions[begin][0]);
        pStart = closure(v);
        pStart->idx = Item::inc();
        items.push_back(pStart);

        queue<Item*> q;
        q.push(pStart);

        vector<Item*> changes;

        while (!q.empty())
        {
            Item* pItem = q.front();
            vector<Production::Item> s;
            symbols(pItem, s);
            for (vector<Production::Item>::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
            {
                Item* pNewItem = NULL;
                if (go(pItem, *i, pNewItem))
                {
                    long n = itemIndex(pNewItem);
                    if (n == -1)
                    {
                        pNewItem->idx = Item::inc();
                        q.push(pNewItem);
                        items.push_back(pNewItem);
                    }
                    else
                    {
                        items[n]->mergeWildCards(pNewItem);
                        changes.push_back_unique(items[n]);
                    }
                    edges[pItem].push_back_unique(Edge(pItem, pNewItem, *i));
                }
            }
            q.pop();
        }

        return true;
    }

    LALR1::Item* LALR1::closure(const vector<LALR1Production>& kernel)
    {
        Item* pItem = Item_Alloc::allocate();
        construct(pItem);
        context.states.insert(pItem);

        queue<LALR1Production> q;

        for (vector<LALR1Production>::const_iterator i = kernel.begin(), m = kernel.end(); i != m; ++i)
        {
            pItem->data.push_back(*i);
            q.push(*i);
        }

        while (!q.empty())
        {
            const LALR1Production& p = q.front();
            if (p.idx < p.right.size() && p.right[p.idx].isNoTerminalSymbol()) // 待约项目
            {
                vector<Production::Item> v;
                firstX(p, v, p.idx + 1);
                for (vector<LALR1Production>::iterator i = inputProductions[p.right[p.idx]].begin(), m = inputProductions[p.right[p.idx]].end(); i != m; ++i)
                {
                    if (i->idx > 0) continue;
                    LALR1Production& item = *i;
                    if (v.empty()) item.wildCards.add_unique(p.wildCards);
                    else
                    {
                        for (vector<Production::Item>::const_iterator j = v.begin(), n = v.end(); j != n; ++j)
                        {
                            item.wildCards.push_back_unique(LALR1Production::Item(j->rule));
                        }
                    }
                    vector<LALR1Production>::iterator j = find(pItem->data.begin(), pItem->data.end(), item);
                    if (j == pItem->data.end())
                    {
                        q.push(item);
                        pItem->data.push_back(item);
                    }
                    else j->wildCards.add_unique(item.wildCards);
                }
            }
            q.pop();
        }

        return pItem;
    }

    void LALR1::firstX(const LALR1Production& p, vector<Production::Item>& v, size_t idx)
    {
        if (idx >= p.right.size()) return;

        first(p, v, idx);
    }

    void LALR1::first(const LALR1Production& p, vector<Production::Item>& v, size_t idx)
    {
#ifdef _DEBUG
        if (idx >= p.right.size())
        {
            throw error<const char*>("position out of right size", __FILE__, __LINE__);
            return;
        }
#endif
        if (p.right[idx].isTermainalSymbol())
        {
            v.push_back_unique(p.right[idx]);
            return;
        }

        for (vector<LALR1Production>::const_iterator i = inputProductions[p.right[idx]].begin(), m = inputProductions[p.right[idx]].end(); i != m; ++i)
        {
            if (i->left == i->right[0]) continue;
            if (i->right[0].isTermainalSymbol())
            {
                v.push_back_unique(i->right[0]);
                continue;
            }
            else
            {
                first(*i, v, 0);
            }
        }
    }

    void LALR1::symbols(Item* pItem, vector<Production::Item>& v)
    {
        for (vector<LALR1Production>::const_iterator i = pItem->data.begin(), m = pItem->data.end(); i != m; ++i)
        {
            if (i->idx < i->right.size()) v.push_back_unique(i->right[i->idx]);
        }
    }

    bool LALR1::go(Item* pItem, const Production::Item& x, Item*& newItem)
    {
        vector<LALR1Production> j;
        for (vector<LALR1Production>::iterator i = pItem->data.begin(), m = pItem->data.end(); i != m; ++i)
        {
            if (i->idx < i->right.size() && i->right[i->idx] == x) j.push_back_unique(i->stepUp());// fromItoJ(*i, j);
        }
        if (j.empty()) return false;

        newItem = closure(j);
        return true;
    }

    void LALR1::fromItoJ(const LALR1Production& p, vector<LALR1Production>& v)
    {
        const Production::Item& temp = p.right[p.idx];
        for (vector<LALR1Production>::iterator i = inputProductions[p.left].begin(), m = inputProductions[p.left].end(); i != m; ++i)
        {
            if (i->left == p.left && i->idx > 0 && i->right[i->idx - 1] == temp)
            {
                //i->wildCards.clear();
                i->wildCards.add(p.wildCards);
                v.push_back(*i);
            }
        }
    }

    long LALR1::itemIndex(Item* pItem)
    {
        for (size_t i = 0, m = items.size(); i < m; ++i)
        {
            if (items[i]->data == pItem->data) return i;
        }
        return -1;
    }
}
