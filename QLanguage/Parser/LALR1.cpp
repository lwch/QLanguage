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
    LALR1::LALR1(const vector<Production>& productions, const Production::Item& start) : begin("begin")
    {
        for (vector<Production>::const_iterator i = productions.begin(), m = productions.end(); i != m; ++i)
        {
            LALR1Production p(*i);
            for (size_t j = 0, n = p.right.size(); j < n; ++j)
            {
                p.idx = j;
                productionMap[p.left][j].push_back(p);
            }
        }
        LALR1Production p(begin);
        p.right.push_back(start);
        productionMap[begin][0].push_back(p);

        p.idx = 1;
        productionMap[begin][1].push_back(p);
    }

    bool LALR1::make()
    {
        productionMap[begin][0][0].wildCards.push_back(LALR1Production::Item());
        Item startItem;
        startItem.data.push_back(productionMap[begin][0][0]);
        closure(startItem.data);
//         for (map<Production::Item, vector<LALR1Production> >::iterator i = pStart->data.begin(), m = pStart->data.end(); i != m; ++i)
//         {
//             if (i->first.name == "begin")
//             {
//                 i->second[0].wildCards.push_back(LALR1Production::Item());
//                 break;
//             }
//         }
//         bool bContinue = true;
//         closure(pStart, bContinue);
//         while (bContinue)
//         {
//             bContinue = false;
//             for (vector<Item*>::iterator i = items.begin(), m = items.end(); i != m; ++i)
//             {
//                 closure(*i, bContinue);
//             }
//         }
        for (vector<LALR1Production>::const_iterator i = startItem.data.begin(), m = startItem.data.end(); i != m; ++i)
        {
            i->print();
        }
        return true;
    }

    void LALR1::closure(vector<LALR1Production>& kernel)
    {
        queue<LALR1Production> q;
        vector<LALR1Production> p;
        for (vector<LALR1Production>::const_iterator i = kernel.begin(), m = kernel.end(); i != m; ++i)
        {
            q.push(*i);
        }
        while (!q.empty())
        {
            q.front().wildCards.clear();
            if (q.front().right[q.front().idx].type == Production::Item::NoTerminalSymbol)
            {
                vector<LALR1Production::Item> v;
                afirst(q.front(), q.front().idx, v);
                //if (q.front().right[q.front().idx].type == Production::Item::NoTerminalSymbol)
                for (vector<LALR1Production>::iterator j = productionMap[q.front().right[q.front().idx]][0].begin(), n = productionMap[q.front().right[q.front().idx]][0].end(); j != n; ++j)
                {
                    LALR1Production& tmp = *j;
                    if (v.size() == 0) tmp.wildCards.add_unique(q.front().wildCards);
                    else tmp.wildCards.add_unique(v);
                    q.front().wildCards.clear();
                    if (p.push_back_unique(tmp)) q.push(tmp);
                    else
                    {
                        for (vector<LALR1Production>::iterator i = p.begin(), m = p.end(); i != m; ++i)
                        {
                            if (*i == tmp)
                            {
                                i->wildCards.add_unique(tmp.wildCards);
                                break;
                            }
                        }
                    }
                }
            }
            q.pop();
        }
        for (vector<LALR1Production>::const_iterator i = p.begin(), m = p.end(); i != m; ++i)
        {
            kernel.push_back(*i);
        }
        //kernel.add(p);
    }

    void LALR1::afirst(const LALR1Production& p, size_t pos, vector<LALR1Production::Item>& v)
    {
        v.clear();
        if (pos + 1 < p.right.size()) return;
        v.add_unique(v);
    }

    void LALR1::first(const LALR1Production& p, size_t pos, vector<LALR1Production::Item>& v)
    {
        v.clear();
        if (p.right[pos].type == Production::Item::TerminalSymbol)
        {
            v.push_back_unique(LALR1Production::Item(p.right[pos].rule));
        }
        else
        {
            queue<Production::Item> q;
            set<Production::Item> s;
            q.push(p.right[pos]);
            s.insert(p.right[pos]);
            while (!q.empty())
            {
                for (vector<LALR1Production>::iterator i = productionMap[q.front()][0].begin(), m = productionMap[q.front()][0].end(); i != m; ++i)
                {
                    LALR1Production& p = *i;
                    Production::Item& k = p.right[0];
                    if (k.type == Production::Item::TerminalSymbol) v.push_back_unique(LALR1Production::Item(k.rule));
                    else if (s.insert(k).second)
                    {
                        for (vector<Production::Item>::size_type j = 0, n = p.right.size(); j < n; ++j)
                        {
                            if (p.right[j].type == Production::Item::TerminalSymbol) break;
                            else if (s.insert(p.right[j]).second) q.push(p.right[j]);
                        }
                    }
                }
                q.pop();
            }
        }
    }

//     void LALR1::closure(Item* pItem, bool& bContinue)
//     {
//         for (map<Production::Item, vector<LALR1Production> >::const_iterator i = pItem->data.begin(), m = pItem->data.end(); i != m; ++i)
//         {
//             for (vector<LALR1Production>::const_iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
//             {
//                 if (j->right[j->idx].type == Production::Item::NoTerminalSymbol) // 计算自生和可传播的展望符
//                 {
//                     vector<LALR1Production::Item> v;
//                     afirst(*j, v);
//                     if (v.size())
//                     {
//                         vector<Production::Item> vts;
//                         appendWildCards(pItem, j->right[j->idx], v, vts, bContinue); // 添加自生展望符
//                         for (vector<Production::Item>::const_iterator k = vts.begin(), o = vts.end(); k != o; ++k)
//                         {
//                             spreadWildCards(go(pItem, *k), v, bContinue); // 传播展望符
//                         }
//                     }
//                     else
//                     {
//                         v = j->wildCards;
//                         for (vector<Edge>::const_iterator k = edges[pItem].begin(), o = edges[pItem].end(); k != o; ++k)
//                         {
//                             spreadWildCards(go(pItem, k->item), v, bContinue);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// 
//     LALR1::Item* LALR1::go(Item* pItem, const Production::Item& x)
//     {
//         for (vector<Edge>::iterator i = edges[pItem].begin(), m = edges[pItem].end(); i != m; ++i)
//         {
//             if (i->item == x) return i->pTo;
//         }
//         throw error<string>("can't find item", __FILE__, __LINE__);
//         return NULL;
//     }
// 
//     void LALR1::afirst(const LALR1Production& p, vector<LALR1Production::Item>& v)
//     {
//         size_t idx = p.idx + 1;
//         if (idx < p.right.size())
//         {
//             if (p.right[idx].type == Production::Item::TerminalSymbol) v.push_back_unique(LALR1Production::Item(p.right[idx].rule));
//             else
//             {
//                 queue<Production::Item> q;
//                 set<Production::Item> s;
//                 q.push(p.right[idx]);
//                 s.insert(p.right[idx]);
//                 while (!q.empty())
//                 {
//                     for (vector<Production>::const_iterator i = lr0.productionMap[q.front()].begin(), m = lr0.productionMap[q.front()].end(); i != m; ++i)
//                     {
//                         if (i->right[0].type == Production::Item::TerminalSymbol) v.push_back_unique(LALR1Production::Item(i->right[0].rule));
//                         else if (s.insert(i->right[0]).second) q.push(i->right[0]);
//                     }
//                     q.pop();
//                 }
//             }
//         }
//     }
// 
//     void LALR1::appendWildCards(Item* pItem, const Production::Item& left, const vector<LALR1Production::Item>& v, vector<Production::Item>& vts, bool& bContinue)
//     {
//         queue<LALR1Production*> q;
//         set<Production::Item> s;
//         s.insert(left);
//         for (vector<LALR1Production>::iterator i = pItem->data[left].begin(), m = pItem->data[left].end(); i != m; ++i)
//         {
//             if (i->idx == 0) q.push(i); // 只发射到0型项目
//         }
//         while (!q.empty())
//         {
//             size_t size = q.front()->wildCards.size();
//             q.front()->wildCards.add_unique(v);
//             if (size != q.front()->wildCards.size()) bContinue = true;
//             if (q.front()->right[0].type == Production::Item::TerminalSymbol) // 只有第一个是终结符的才需要传播吗？
//             {
//                 vts.push_back_unique(q.front()->right[0]);
//             }
//             else if (s.insert(q.front()->right[0]).second)
//             {
//                 for (vector<LALR1Production>::iterator i = pItem->data[q.front()->right[0]].begin(), m = pItem->data[q.front()->right[0]].end(); i != m; ++i)
//                 {
//                     if (i->idx == 0) q.push(i); // 只发射到0型项目
//                 }
//             }
//             q.pop();
//         }
//     }
// 
//     void LALR1::spreadWildCards(Item* pItem, const vector<LALR1Production::Item>& v, bool& bContinue)
//     {
//         for (map<Production::Item, vector<LALR1Production> >::iterator i = pItem->data.begin(), m = pItem->data.end(); i != m; ++i)
//         {
//             for (vector<LALR1Production>::iterator j = i->second.begin(), n = i->second.end(); j != n; ++j)
//             {
//                 size_t size = j->wildCards.size();
//                 j->wildCards.add_unique(v);
//                 if (size != j->wildCards.size()) bContinue = true;
//             }
//         }
//     }

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
        fstream fs(path, fstream::out | fstream::text);
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
        fs << string::format("start: %03d\n\n", pStart->idx);
        for (hashset<Item*>::const_iterator i = s.begin(), m = s.end(); i != m; ++i)
        {
            fs << string::format("Item: %d\n", (*i)->idx);
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
