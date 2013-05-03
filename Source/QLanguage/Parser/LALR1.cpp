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
            vector<Production::Item> vs;
            closure(*i, vs);
            vector<Production::Item> vts, vns;
            select_into(vs, vts, isVT, addV);
            select_into(vs, vns, isVN, addV);
            this->vts.add_unique(vts);
            this->vns.add_unique(vns);
            for (vector<LALR1Production>::const_iterator j = (*i)->data.begin(), n = (*i)->data.end(); j != n; ++j)
            {
                for (vector<Edge>::iterator k = edges[*i].begin(), o = edges[*i].end(); k != o; ++k)
                {
                    for (vector<Production::Item>::const_iterator a = vs.begin(), b = vs.end(); a != b; ++a)
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
        vns.push_back_unique(lr0.start);
        sort(vts.begin(), vts.end(), compare_production_item_idx);
        sort(vns.begin(), vns.end(), compare_production_item_idx);
        sort(items.begin(), items.end(), compare_item_idx);
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
            if (temp.idx < temp.right.size())
            {
                if (temp.right[temp.idx].isNoTerminalSymbol())
                {
                    for (vector<Production>::const_iterator i = lr0.productionMap[temp.right[temp.idx]].begin(), m = lr0.productionMap[temp.right[temp.idx]].end(); i != m; ++i)
                    {
                        LALR1Production p(i->left, i->right);
                        p.wildCards.add_unique(v);
                        pKernel->data.push_back_unique(p);
                        vts.push_back_unique(p.right[p.idx]);
                    }
                }
                else vts.push_back_unique(temp.right[temp.idx]);
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
        table.initialize(items.size() * (vns.size() + vts.size() + 1));
        for (vector<Item*>::const_iterator i = items.begin(), m = items.end(); i != m; ++i)
        {
            for (vector<LALR1Production>::const_iterator j = (*i)->data.begin(), n = (*i)->data.end(); j != n; ++j)
            {
                const LALR1Production& p = *j;
                if (p.idx < p.right.size())
                {
                    const Production::Item& c = p.right[p.idx];
                    fillTable(*i, c);
                }
                else
                {
                    if (p.left == lr0.begin)
                    {
                        fillTable((*i)->idx, vts.size(), pair<uchar, ushort>('A', 0));
                    }
                    else
                    {
                        for (vector<LALR1Production::Item>::const_iterator k = p.wildCards.begin(), o = p.wildCards.end(); k != o; ++k)
                        {
                            long q = index_of(vts.begin(), vts.end(), *k, compare_production_item);
                            long r = index_of(lr0.inputProductions.begin(), lr0.inputProductions.end(), *j, compare_production);
                            if (r == -1) continue;
                            if (k->type == LALR1Production::Item::End) q = vts.size();
                            if (q == -1) continue;
                            fillTable((*i)->idx, q, pair<uchar, ushort>('R', (ushort)r));
                        }
                    }
                }
            }
        }
    }

    void LALR1::fillTable(Item* pItem, const Production::Item& c)
    {
        if (c.name == "begin")
        {
            vector<Edge>::const_iterator first = edges[pItem].begin();
            vector<Edge>::const_iterator last = edges[pItem].end();

            vector<Edge>::const_iterator i = find(first, last, c, compare_edge);

            if (i == last) return;

            fillTable(pItem->idx, vts.size(), pair<uchar, ushort>(0, i->pTo->idx));
        }
        else if (c.isTermainalSymbol())
        {
            long idx = index_of(vts.begin(), vts.end(), c);
            vector<Edge>::const_iterator first = edges[pItem].begin();
            vector<Edge>::const_iterator last = edges[pItem].end();

            vector<Edge>::const_iterator i = find(first, last, c, compare_edge);

            if (idx == -1 || i == last) return;

            fillTable(pItem->idx, idx, pair<uchar, ushort>('S', i->pTo->idx));
        }
        else
        {
            long idx = index_of(vns.begin(), vns.end(), c);
            vector<Edge>::const_iterator first = edges[pItem].begin();
            vector<Edge>::const_iterator last = edges[pItem].end();

            vector<Edge>::const_iterator i = find(first, last, c, compare_edge);

            if (idx == -1 || i == last) return;

            fillTable(pItem->idx, vts.size() + 1 + idx, pair<uchar, ushort>(0, i->pTo->idx));
        }
    }

    void LALR1::fillTable(uint iLine, uint iChar, const pair<uchar, ushort>& p)
    {
        pair<uchar, ushort>& pa = table[iLine * (vts.size() + 1 + vns.size()) + iChar];
        if (pa.first == 0 && pa.second == 0) pa = p;
        else if (pa.first == 'R' && p.first == 'S') pa = p;
    }

    inline const bool LALR1::compare_edge(const Edge& e, const Production::Item& i)
    {
        return e.item == i;
    }

    inline const bool LALR1::compare_production_item(const Production::Item& i1, const LALR1Production::Item& i2)
    {
        return i1.rule == i2.rule;
    }

    inline const bool LALR1::compare_production(const Production& p1, const LALR1Production& p2)
    {
        return p1.left == p2.left && p1.right == p2.right;
    }

    inline const bool LALR1::compare_item_idx(const Item* i1, const Item* i2)
    {
        return i1->idx < i2->idx;
    }

    inline const bool LALR1::compare_production_item_idx(const Production::Item& i1, const Production::Item& i2)
    {
        return i1.index < i2.index;
    }

    inline const bool LALR1::isVN(const Production::Item& i)
    {
        return i.isNoTerminalSymbol();
    }

    inline const bool LALR1::isVT(const Production::Item& i)
    {
        return i.isTermainalSymbol();
    }

    inline void LALR1::addV(vector<Production::Item>& container, const Production::Item& i)
    {
        container.push_back_unique(i);
    }

    void LALR1::print(Library::ostream& stream)
    {
#ifdef _DEBUG
        stream << "-------- LALR(1) Start --------" << endl;
        stream << "----- State Machine Start -----" << endl;
        for (hashmap<Item*, vector<Edge> >::const_iterator i = edges.begin(), m = edges.end(); i != m; ++i)
        {
            for (vector<Edge>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
            {
                j->print(stream);
            }
        }
        stream << string::format("start: %03d", pStart->idx) << endl << endl;
        for (vector<Item*>::const_iterator i = items.begin(), m = items.end(); i != m; ++i)
        {
            stream << string::format("Item: %d", (*i)->idx) << endl;
            for (vector<LALR1Production>::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                k->print(stream);
            }
            stream << endl;
        }
        stream << "------ State Machine End ------" << endl;
        stream << "------ Parse Table Start ------" << endl;
        stream << string::format("Action Count: %d", vts.size() + 1)
               << " "
               << string::format("GoTo Count: %d", vns.size())
               << endl;
        for (vector<Production::Item>::const_iterator i = vts.begin(), m = vts.end(); i != m; ++i)
        {
            stream << "\t" << i->index;
        }
        stream << "\t$";
        for (vector<Production::Item>::const_iterator i = vns.begin(), m = vns.end(); i != m; ++i)
        {
            stream << "\t" << i->index;
        }
        stream << endl;
        uint idx = 0;
        for (vector<Item*>::const_iterator i = items.begin(), m = items.end(); i != m; ++i)
        {
            Item* const pItem = *i;
            stream << pItem->idx;
            for (vector<Production::Item>::const_iterator j = vts.begin(), n = vts.end(); j != n; ++j)
            {
                const pair<uchar, ushort>& pa = table[idx++];
                if (pa.first) stream << "\t" << string::format("%c%u", pa.first, pa.second);
                else stream << "\tε";
            }
            const pair<uchar, ushort>& pa = table[idx++];
            if (pa.first) stream << "\t" << string::format("%c%u", pa.first, pa.second);
            else stream << "\tε";
            for (vector<Production::Item>::const_iterator j = vns.begin(), n = vns.end(); j != n; ++j)
            {
                const pair<uchar, ushort>& pa = table[idx++];
                if (pa.second) stream << "\t" << pa.second;
                else stream << "\tε";
            }
            stream << endl;
        }
        stream << "VTS:" << endl;
        for (vector<Production::Item>::const_iterator i = vts.begin(), m = vts.end(); i != m; ++i)
        {
            stream << i->index << " : " << i->rule.showName << endl;
        }
        stream << "VNS:" << endl;
        for (vector<Production::Item>::const_iterator i = vns.begin(), m = vns.end(); i != m; ++i)
        {
            stream << i->index << " : " << i->name << endl;
        }
        stream << "------- Parse Table End -------" << endl;
        stream << "--------- LALR(1) End ---------" << endl;
#endif
    }
}
