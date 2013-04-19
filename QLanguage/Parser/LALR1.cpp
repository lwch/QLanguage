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
#include "../../QCore/Library/fstream.h"

#include "LALR1.h"

namespace QLanguage
{
    LALR1::LALR1(LR0& lr0) : lr0(lr0)
    {
    }

    bool LALR1::make()
    {
        items.clear();
        items.reserve(lr0.items.size());
        hashmap<LR0::Item*, Item*> hm;
        for (hashmap<LR0::Item*, vector<LR0::Edge> >::const_iterator i = lr0.edges.begin(), m = lr0.edges.end(); i != m; ++i)
        {
            for (vector<LR0::Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
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
        for (vector<LALR1Production>::iterator i = pStart->data.begin(), m = pStart->data.end(); i != m; ++i)
        {
            if (i->left.name == "begin") i->wildCards.push_back(LALR1Production::Item());
        }
        for (vector<Item*>::const_iterator i = items.begin(), m = items.end(); i != m; ++i)
        {
            vector<Production::Item> vts;
            closure(*i, vts);
            vns.add_unique(vts, isVN);
            vts.add_unique(vts, isVT);
            for (vector<LALR1Production>::const_iterator j = (*i)->data.begin(), n = (*i)->data.end(); j != n; ++j)
            {
                for (vector<Edge>::iterator k = edges[*i].begin(), o = edges[*i].end(); k != o; ++k)
                {
                    for (vector<Production::Item>::const_iterator a = vts.begin(), b = vts.end(); a != b; ++a)
                    {
                        if (k->item == *a) go(*j, k->pTo, *a);
                    }
                }
            }
        }
        bool bContinue = true;
        while (bContinue)
        {
            bContinue = false;
            for (vector<Item*>::iterator i = items.begin(), m = items.end(); i != m; ++i)
            {
                for (vector<Edge>::iterator j = edges[*i].begin(), n = edges[*i].end(); j != n; ++j)
                {
                    for (vector<LALR1Production>::iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
                    {
                        for (vector<LALR1Production>::iterator a = j->pTo->data.begin(), b = j->pTo->data.end(); a != b; ++a)
                        {
                            size_t size = a->wildCards.size();
                            a->wildCards.add_unique(k->wildCards);
                            if (a->wildCards.size() > size) bContinue = true;
                        }
                    }
                }
            }
        }
        buildParserTable();
        return true;
    }

    void LALR1::closure(Item* pKernel, vector<Production::Item>& vts)
    {
        size_t iCount = 0;
        while (iCount < pKernel->data.size())
        {
            LALR1Production& temp = pKernel->data[iCount];
            vector<LALR1Production::Item> v;
            first(temp.right.begin() + temp.idx + 1, temp.right.end(), temp.wildCards, v);
            if (temp.idx < temp.right.size() && temp.right[temp.idx].isNoTerminalSymbol())
            {
                for (vector<Production>::const_iterator i = lr0.productionMap[temp.right[temp.idx]].begin(), m = lr0.productionMap[temp.right[temp.idx]].end(); i != m; ++i)
                {
                    LALR1Production p(i->left, i->right);
                    p.wildCards.add_unique(v);
                    pKernel->data.push_back_unique(p);
                    vts.push_back_unique(p.right[p.idx]);
                }
            }
            ++iCount;
        }
    }

    void LALR1::go(const LALR1Production& p, Item* pTo, const Production::Item& item)
    {
        for (vector<LALR1Production>::iterator i = pTo->data.begin(), m = pTo->data.end(); i != m; ++i)
        {
            if (i->idx > 0 && i->right[i->idx - 1] == item) i->wildCards.add_unique(p.wildCards);
        }
    }

    void LALR1::first(vector<Production::Item>::const_iterator first, vector<Production::Item>::const_iterator last, const vector<LALR1Production::Item>& wildCards, vector<LALR1Production::Item>& v)
    {
        if (first >= last) v.add(wildCards);
        else if (first->isTermainalSymbol()) v.push_back_unique(first->rule);
        else
        {
            queue<Production::Item> q;
            set<Production::Item> s;
            for (; first != last; ++first)
            {
                if (first->isTermainalSymbol()) break;
                else
                {
                    q.push(*first);
                }
            }
            bool bAddWildCards = false;
            if (first == last) bAddWildCards = true;
            while (!q.empty())
            {
                for (vector<Production>::const_iterator i = lr0.productionMap[q.front()].begin(), m = lr0.productionMap[q.front()].end(); i != m; ++i)
                {
                    if (i->right[0].isTermainalSymbol()) v.push_back_unique(i->right[0].rule);
                    else
                    {
                        if (s.insert(i->right[0]).second) q.push(i->right[0]);
                    }
                }
                q.pop();
            }
            if (bAddWildCards) v.add_unique(wildCards);
        }
    }

    void LALR1::buildParserTable()
    {
        typedef pair<uchar, uint> pair_type;
        table.initialize(items.size() * (vns.size() + vts.size() + 1));
        for (vector<Item*>::const_iterator i = items.begin(), m = items.end(); i != m; ++i)
        {
            for (vector<LALR1Production>::const_iterator j = (*i)->data.begin(), n = (*i)->data.end(); j != n; ++j)
            {
                const LALR1Production& p = *j;
                const vector<Production::Item>& right = p.right;
                const uint idx = p.idx;
                const size_t m = right.size();
                if (idx < m)
                {
                    const Production::Item& c = right[idx];
                    fillTable(*i, c);
                }
                else
                {
//                     if (p.left.name == "begin" && p.idx == 1) fillTable(i->idx, pair_type('A', 0));
//                     else
//                     {
//                         for (vector<LALR1Production::Item>::const_iterator k = p.wildCards.begin(), o = p.wildCards.end(); k != o; ++k)
//                         {
//                             fillTable(i->idx, *k);
//                         }
//                     }
                }
            }
        }
    }

    void LALR1::fillTable(Item* pItem, const Production::Item& c)
    {
        if (c.isTermainalSymbol())
        {
            vector<Edge>::const_iterator first = edges[pItem].begin();
            vector<Edge>::const_iterator last = edges[pItem].end();

            vector<Edge>::const_iterator i = find(first, last, c, compare_edge);
        }
        else
        {

        }
    }

    inline const bool LALR1::compare_edge(const Edge& e, const Production::Item& i)
    {
        return e.item == i;
    }

    inline const bool LALR1::isVN(const Production::Item& i)
    {
        return i.isNoTerminalSymbol();
    }

    inline const bool LALR1::isVT(const Production::Item& i)
    {
        return i.isTermainalSymbol();
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
                j->print();
                s.insert(j->pFrom);
                s.insert(j->pTo);
            }
        }
        printf("start: %03d\n\n", pStart->idx);
        for (hashset<Item*>::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
        {
            printf("Item: %d\n", (*i)->idx);
            for (vector<LALR1Production>::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                k->print();
            }
            printf("\n");
        }
#endif
        printf("--------- LALR(1) End ---------\n");
    }

    void LALR1::print(const string& path)
    {
        hashset<Item*> s;
        fstream fs(path, fstream::out);
        fs << "-------- LALR(1) Start --------" << endl;
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
            for (vector<LALR1Production>::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                k->print(fs);
            }
            fs << endl;
        }
#endif
        fs << "--------- LALR(1) End ---------" << endl;
    }
}
