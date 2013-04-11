/********************************************************************
	created:	2013/04/01
	created:	1:4:2013   17:10
	filename: 	\QLanguage\Parser\LALR1.cpp
	file path:	\QLanguage\Parser
	file base:	LALR1
	file ext:	cpp
	author:		lwch
	
	purpose:	http://dec3.jlu.edu.cn/webcourse/T000171/chap4/4-5-7.html
*********************************************************************/
#include "../../QCore/Library/hashset.h"

#include "LALR1.h"

namespace QLanguage
{
    LALR1::LALR1(LR0& lr0) : lr0(lr0)
    {
        hashmap<LR0::Item*, Item*> hm;
        for (hashmap<LR0::Item*, vector<LR0::Edge> >::iterator i = lr0.edges.begin(), m = lr0.edges.end(); i != m; ++i)
        {
            for (vector<LR0::Edge>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                if (hm[j->pFrom] == NULL)
                {
                    hm[j->pFrom] = Item_Alloc::allocate();
                    construct(hm[j->pFrom], *j->pFrom);
                    context.states.insert(hm[j->pFrom]);
                    items.push_back(hm[j->pFrom]);
                }

                if (hm[j->pTo] == NULL)
                {
                    hm[j->pTo] = Item_Alloc::allocate();
                    construct(hm[j->pTo], *j->pTo);
                    context.states.insert(hm[j->pTo]);
                    items.push_back(hm[j->pTo]);
                }
                edges[hm[j->pFrom]].push_back(Edge(hm[j->pFrom], hm[j->pTo], j->item));
            }
        }
        pStart = hm[lr0.pStart];
        for (set<LR0::Item*>::const_iterator i = lr0.pEnds.begin(), m = lr0.pEnds.end(); i != m; ++i)
        {
            pEnd.insert(hm[*i]);
        }
    }

    bool LALR1::make()
    {
        for (map<Production::Item, vector<LALR1Production> >::iterator i = pStart->data.begin(), m = pStart->data.end(); i != m; ++i)
        {
            if (i->first.name == "begin")
            {
                i->second[0].wildCards.push_back(LALR1Production::Item());
            }
        }
        bool bContinue = true;
        closure(pStart, bContinue);
        while (bContinue)
        {
            bContinue = false;
            for (vector<Item*>::iterator i = items.begin(), m = items.end(); i != m; ++i)
            {
                closure(*i, bContinue);
            }
        }
        return true;
    }

    void LALR1::closure(Item* pItem, bool& bContinue)
    {
        for (map<Production::Item, vector<LALR1Production> >::const_iterator i = pItem->data.begin(), m = pItem->data.end(); i != m; ++i)
        {
            for (vector<LALR1Production>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                if (j->right[j->idx].type == Production::Item::NoTerminalSymbol) // 计算自生和可传播的展望符
                {
                    vector<LALR1Production::Item> v;
                    afirst(*j, v);
                    if (v.size())
                    {
                        vector<Production::Item> vts;
                        appendWildCards(pItem, j->right[j->idx], v, vts, bContinue); // 添加自生展望符
                        for (vector<Production::Item>::const_iterator k = vts.begin(), o = vts.end(); k != o; ++k)
                        {
                            spreadWildCards(go(pItem, *k), v, bContinue); // 传播展望符
                        }
                    }
                    else
                    {
                        v = j->wildCards;
                        for (vector<Edge>::const_iterator k = edges[pItem].begin(), o = edges[pItem].end(); k != o; ++k)
                        {
                            spreadWildCards(go(pItem, k->item), v, bContinue);
                        }
                    }
                }
            }
        }
    }

    LALR1::Item* LALR1::go(Item* pItem, const Production::Item& x)
    {
        for (vector<Edge>::iterator i = edges[pItem].begin(), m = edges[pItem].end(); i != m; ++i)
        {
            if (i->item == x) return i->pTo;
        }
        throw error<string>("can't find item", __FILE__, __LINE__);
        return NULL;
    }

    void LALR1::afirst(const LALR1Production& p, vector<LALR1Production::Item>& v)
    {
        size_t idx = p.idx + 1;
        if (idx < p.right.size())
        {
            if (p.right[idx].type == Production::Item::TerminalSymbol) v.push_back_unique(LALR1Production::Item(p.right[idx].rule));
            else
            {
                queue<Production::Item> q;
                set<Production::Item> s;
                q.push(p.right[idx]);
                s.insert(p.right[idx]);
                while (!q.empty())
                {
                    for (vector<Production>::const_iterator i = lr0.productionMap[q.front()].begin(), m = lr0.productionMap[q.front()].end(); i != m; ++i)
                    {
                        if (i->right[0].type == Production::Item::TerminalSymbol) v.push_back_unique(LALR1Production::Item(i->right[0].rule));
                        else if (s.insert(i->right[0]).second) q.push(i->right[0]);
                    }
                    q.pop();
                }
            }
        }
    }

    void LALR1::appendWildCards(Item* pItem, const Production::Item& left, const vector<LALR1Production::Item>& v, vector<Production::Item>& vts, bool& bContinue)
    {
        for (vector<LALR1Production>::iterator i = pItem->data[left].begin(), m = pItem->data[left].end(); i != m; ++i)
        {
            if (i->idx == 0) // 只发射到0型项目
            {
                size_t size = i->wildCards.size();
                i->wildCards.add_unique(v);
                if (size != i->wildCards.size()) bContinue = true;
                if (i->right[0].type == Production::Item::TerminalSymbol) // 只有第一个是终结符的才需要传播吗？
                {
                    vts.push_back_unique(i->right[0]);
                }
            }
        }
    }

    void LALR1::spreadWildCards(Item* pItem, const vector<LALR1Production::Item>& v, bool& bContinue)
    {
        for (map<Production::Item, vector<LALR1Production> >::iterator i = pItem->data.begin(), m = pItem->data.end(); i != m; ++i)
        {
            for (vector<LALR1Production>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                size_t size = j->wildCards.size();
                j->wildCards.add_unique(v);
                if (size != j->wildCards.size()) bContinue = true;
            }
        }
    }

    void LALR1::print()
    {
        hashset<Item*> s;
        printf("-------- LALR(1) Start --------\n");
#ifdef _DEBUG
        for (hashmap<Item*, vector<Edge> >::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
        {
            for (vector<Edge>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                printf("%03d -> %03d", j->pFrom->idx, j->pTo->idx);
                if (j->item.type == Production::Item::TerminalSymbol)
                {
                    printf("\n\n");
                    j->item.rule.printEpsilonNFA();
                }
                else printf("(%s)\n", j->item.name.c_str());
                printf("\n");
                s.insert(j->pFrom);
                s.insert(j->pTo);
            }
        }
        printf("start: %03d\n\n", pStart->idx);
        int j = 0;
        for (hashset<Item*>::const_iterator i = s.begin(), m = s.end(); i != m; ++i, ++j)
        {
            printf("Item: %d\n", (*i)->idx);
            for (map<Production::Item, vector<LALR1Production> >::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                for (vector<LALR1Production>::const_iterator l = k->second.begin(), p = k->second.end(); l != p; ++l)
                {
                    printf("%s -> ", l->left.name.c_str());
                    uint c = 0;
                    for (vector<Production::Item>::const_iterator a = l->right.begin(), b = l->right.end(); a != b; ++a, ++c)
                    {
                        if (c == l->idx) printf(". ");
                        if (a->type == Production::Item::NoTerminalSymbol) printf("%s ", a->name.c_str());
                        else printf("VN ");
                    }
                    printf("\nwildCards:\n");
                    for (vector<LALR1Production::Item>::const_iterator a = l->wildCards.begin(), b = l->wildCards.end(); a != b; ++a)
                    {
                        if (a->type == LALR1Production::Item::Rule)
                        {
                            a->rule.printEpsilonNFA();
                        }
                        else printf("#\n");
                    }
                    printf("\n");
                }
            }
            printf("\n");
        }
#endif
        printf("--------- LALR(1) End ---------\n");
    }
}
