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

        typedef pair<Item*, vector<Production::Item> > pair_type;
        vector<Production::Item> v;
        pStart = closure(kernel);
        items.push_back(pStart);
        vs(pStart, v);
        queue<pair_type> s;
        s.push(pair_type(pStart, v));

        bool bContinue = true;
        while (bContinue && !s.empty())
        {
            pair_type item = s.front();
            s.pop();
            for (vector<Production::Item>::const_iterator i = item.second.begin(), m = item.second.end(); i != m; ++i)
            {
                pair<Item*, bool> pNewItem = go(item.first, *i);
                edges[item.first].push_back(Edge(item.first, pNewItem.first, *i));
                if (!pNewItem.second) continue;
                items.push_back(pNewItem.first);
                for (map<LR0Production::Item, vector<LR0Production> >::iterator j = pNewItem.first->data.begin(), n = pNewItem.first->data.end(); j != n; ++j)
                {
                    for (vector<LR0Production>::iterator k = j->second.begin(), o = j->second.end(); k != o; ++k)
                    {
                        if (k->idx >= 1 && k->right[k->idx - 1] == start) pEnds.insert(pNewItem.first);
                    }
                }
                v.clear();
                vs(pNewItem.first, v);
                s.push(pair_type(pNewItem.first, v));
            }
        }

        return true;
    }

    LR0::Item* LR0::closure(const vector<LR0Production>& x)
    {
        Item* pItem = Item_Alloc::allocate();
        context.states.insert(pItem);
        construct(pItem);
        
        queue<LR0Production::Item> q;
        for (vector<LR0Production>::const_iterator i = x.begin(), m = x.end(); i != m; ++i)
        {
            pItem->data[i->left].push_back_unique(*i);
            q.push_unique(i->right[i->idx]);
        }

        bool bContinue = true;
        while (bContinue && !q.empty())
        {
            LR0Production::Item& i = q.front();
            for (vector<Production>::const_iterator j = productionMap[i].begin(), n = productionMap[i].end(); j != n; ++j)
            {
                if (!pItem->data[j->left].push_back_unique(*j))
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

    void LR0::closure(const LR0Production& x, map<LR0Production::Item, vector<LR0Production> >& y)
    {
        y[x.left].push_back_unique(x);
        if (x.idx == x.right.size()) return;
        if (x.right[x.idx].type == LR0Production::Item::NoTerminalSymbol) // 若是非终结符
        {
            // 将这个非终结符所对应的所有产生式加入其中
            for (vector<Production>::const_iterator j = productionMap[x.right[x.idx]].begin(), n = productionMap[x.right[x.idx]].end(); j != n; ++j)
            {
                if (!y[j->left].push_back_unique(*j)) break;
            }
        }
    }

    pair<LR0::Item*, bool> LR0::go(Item* i, const Production::Item& x)
    {
        Item* pItem = Item_Alloc::allocate();
        construct(pItem);

        for (map<LR0Production::Item, vector<LR0Production> >::iterator j = i->data.begin(), n = i->data.end(); j != n; ++j)
        {
            for (vector<LR0Production>::iterator k = j->second.begin(), o = j->second.end(); k != o; ++k)
            {
                if (k->idx < k->right.size() - 1 && k->right[k->idx] == x)
                {
                    if (k->idx < k->right.size())
                    {
                        LR0Production p = k->stepUp();
                        p.bKernel = true;
                        closure(p, pItem->data);
                    }
                }
            }
        }

        Item* pOldItem = NULL;

        if (context.isItemExists(pItem, pOldItem))
        {
            destruct(pItem, has_destruct(*pItem));
            Item_Alloc::deallocate(pItem);
            return pair<Item*, bool>(pOldItem, false);
        }
        else context.states.insert(pItem);

        return pair<Item*, bool>(pItem, true);
    }

    void LR0::vs(Item* i, vector<Production::Item>& v)
    {
        for (map<LR0Production::Item, vector<LR0Production> >::const_iterator j = i->data.begin(), n = i->data.end(); j != n; ++j)
        {
            for (vector<LR0Production>::const_iterator k = j->second.begin(), o = j->second.end(); k != o; ++k)
            {
                if (k->idx < k->right.size()) v.push_back_unique(k->right[k->idx]);
            }
        }
    }

    void LR0::print()
    {
        hashset<Item*> s;
        printf("-------- LR(0) Start --------\n");
#ifdef _DEBUG
        for (hashmap<Item*, vector<Edge> >::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
        {
            for (vector<Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                j->print();
                s.insert(j->pFrom);
                s.insert(j->pTo);
            }
        }
        printf("start: %03d\n\n", pStart->idx);
        for (hashset<Item*>::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
        {
            printf("Item: %d\n", (*i)->idx);
            for (map<Production::Item, vector<LR0Production> >::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                for (vector<LR0Production>::const_iterator l = k->second.begin(), p = k->second.end(); l != p; ++l)
                {
                    l->print();
                }
            }
            printf("\n");
        }
#endif
        printf("--------- LR(0) End ---------\n");
    }

    void LR0::print(const string& path)
    {
        hashset<Item*> s;
        fstream fs(path, fstream::out);
        fs << "-------- LR(0) Start --------" << endl;
#ifdef _DEBUG
        for (hashmap<Item*, vector<Edge> >::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
        {
            for (vector<Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                j->print(fs);
                s.insert(j->pFrom);
                s.insert(j->pTo);
            }
        }
        fs << string::format("start: %03d", pStart->idx) << endl << endl;
        for (hashset<Item*>::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
        {
            fs << string::format("Item: %d", (*i)->idx) << endl;
            for (map<Production::Item, vector<LR0Production> >::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                for (vector<LR0Production>::const_iterator l = k->second.begin(), p = k->second.end(); l != p; ++l)
                {
                    l->print(fs);
                }
            }
            fs << endl;
        }
#endif
        fs << "--------- LR(0) End ---------" << endl;
    }
}
