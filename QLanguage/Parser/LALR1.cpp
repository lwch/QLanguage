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
        kernels.clear();
        kernels.reserve(lr0.items.size());
        for (vector<LR0::Item*>::const_iterator i = lr0.items.begin(), m = lr0.items.end(); i != m; ++i)
        {
            Item* pItem = allocator<Item>::allocate();
            construct(pItem, *i);
            context.states.insert(pItem);

            kernels.push_back(pItem);
        }
        for (vector<Item*>::const_iterator i = kernels.begin(), m = kernels.end(); i != m; ++i)
        {
            Item* pItem = closure(*i);
        }
        return true;
    }

    LALR1::Item* LALR1::closure(Item* pKernel)
    {
        Item* pItem = allocator<Item>::allocate();
        construct(pItem);
        context.states.insert(pItem);
        pItem->data.add(pKernel->data);

        size_t iCount = 0;
        while (iCount < pItem->data.size())
        {
            LALR1Production& temp = pItem->data[iCount];
            vector<LALR1Production::Item> v;
            first(temp.right.begin() + temp.idx, temp.right.end(), temp.wildCards, v);
            if (temp.idx < temp.right.size() && temp.right[temp.idx].isNoTerminalSymbol())
            {
                for (vector<Production>::const_iterator i = lr0.productionMap[temp.right[temp.idx]].begin(), m = lr0.productionMap[temp.right[temp.idx]].end(); i != m; ++i)
                {
                    LALR1Production p(i->left, i->right);
                    p.wildCards.add_unique(v);
                    pItem->data.push_back_unique(p);
                }
            }
            ++iCount;
        }
        return pItem;
    }

    void LALR1::first(vector<Production::Item>::const_iterator first, vector<Production::Item>::const_iterator last, const vector<LALR1Production::Item>& wildCards, vector<LALR1Production::Item>& v)
    {
        if (first->isTermainalSymbol()) v.push_back_unique(first->rule);
        else
        {
            for (; first != last; ++first)
            {
                if (first->isNoTerminalSymbol()) break;
                else
                {
                    for (vector<Production>::const_iterator i = lr0.productionMap[*first].begin(), m = lr0.productionMap[*first].end(); i != m; ++i)
                    {
                        this->first(i->right.begin(), i->right.end(), wildCards, v);
                    }
                }
            }
            if (first == last) v.add_unique(wildCards);
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
