/********************************************************************
	created:	2013/05/03
	created:	3:5:2013   11:01
	filename: 	\QLanguage\Parser\LALR1ParserTable.cpp
	file path:	\QLanguage\Parser
	file base:	LALR1ParserTable
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <time.h>

#include "../../QCore/Library/system.h"

#include "LALR1.h"

#define PARSER_TABLE "QPARSERGENERATOR PARSERTABLE"

namespace QLanguage
{
    bool LALR1::buildParserTable()
    {
        table.initialize(items.size() * (vts.size() + 1 + vns.size()));
        vector<pair<uchar, ushort> >::iterator ptr = table.begin();
        for (vector<Item*>::const_iterator i = items.begin(), m = items.end(); i != m; ++i)
        {
            vector<pair<uchar, ushort> >::iterator pt = ptr;
            for (vector<LALR1Production>::const_iterator k = (*i)->data.begin(), o = (*i)->data.end(); k != o; ++k)
            {
                const LALR1Production& p = *k;
                ptr = pt;
                for (size_t j = 0, n = vts.size(); j < n; ++j, ++ptr)
                {
                    if (p.idx == p.right.size() && find(p.wildCards.begin(), p.wildCards.end(), vts[j]) != p.wildCards.end())
                    {
                        ptr->first = 'R';
                        ptr->second = (ushort)index_of(_rules.begin(), _rules.end(), p);
                    }
                    vector<Edge>::const_iterator k = find(edges[*i].begin(), edges[*i].end(), vts[j], compare_edge_item_is);
                    if (k != edges[*i].end())
                    {
                        ptr->first = 'S';
                        ptr->second = k->pTo->idx;
                    }
                }
                if (p.idx == p.right.size() && find(p.wildCards.begin(), p.wildCards.end(), LALR1Production::Item()) != p.wildCards.end())
                {
                    if (p.left == begin && p.idx == p.right.size()) ptr->first = 'A';
                    else
                    {
                        ptr->first = 'R';
                        ptr->second = (ushort)index_of(_rules.begin(), _rules.end(), p);
                    }
                }
                ++ptr;
                for (size_t j = 0, n = vns.size(); j < n; ++j, ++ptr)
                {
                    vector<Edge>::const_iterator k = find(edges[*i].begin(), edges[*i].end(), vns[j], compare_edge_item_is);
                    if (k != edges[*i].end()) ptr->second = k->pTo->idx;
                }
            }
        }
        return true;
    }

    bool LALR1::compare_edge_item_is(const Edge& e, const Production::Item& x)
    {
        return e.item == x;
    }

    void LALR1::print(Library::ostream& stream)
    {
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
            stream << i->index << " : "
                   << i->rule.showName
                   << endl;
        }
        stream << "VNS:" << endl;
        for (vector<Production::Item>::const_iterator i = vns.begin(), m = vns.end(); i != m; ++i)
        {
            stream << i->index << " : "
                   << i->name
                   << endl;
        }
        stream << "------- Parse Table End -------" << endl;
        stream << "--------- LALR(1) End ---------" << endl;
    }

    void LALR1::printRules(Library::ostream& stream)
    {
        for (size_t i = 0, m = _rules.size(); i < m; ++i)
        {
            stream << i << " : ";
            _rules[i].print(stream);
        }
    }

    bool LALR1::output(const string& path)
    {
        time_t t;
        time(&t);
        uchar version = 1;
        fstream stream(path, fstream::out | fstream::binary);
        stream << PARSER_TABLE << version << sizeof(t) << t << vts.size() << vns.size() << _rules.size() << items.size() << this->pStart->idx;
        for (vector<pair<uchar, ushort> >::const_iterator i = table.begin(), m = table.end(); i != m; ++i)
        {
            stream << i->first << i->second;
        }
        for (vector<Production::Item>::const_iterator i = vts.begin(), m = vts.end(); i != m; ++i)
        {
            if (!i->output(stream)) return false;
        }
        for (vector<Production::Item>::const_iterator i = vns.begin(), m = vns.end(); i != m; ++i)
        {
            if (!i->output(stream)) return false;
        }
        for (vector<Production>::const_iterator i = _rules.begin(), m = _rules.end(); i != m; ++i)
        {
            if (!i->output(stream)) return false;
        }
        for (vector<Item*>::const_iterator i = items.begin(), m = items.end(); i != m; ++i)
        {
            stream << (*i)->idx;
        }
        return true;
    }

    bool LALR1::parse(const list<Lexer::Token>& l, BasicParser* pParser)
    {
        stack<ushort> status;
        status.push(pStart->idx);
        list<Lexer::Token> tokens = l;
        long idx = 0;
        while (!tokens.empty())
        {
            Lexer::Token& tk = tokens.front();
            idx = index_of_vt(tk.data, idx);
            if (idx == -1)
            {
                throw error<const char*>("get action error", __FILE__, __LINE__);
                return false;
            }
            const pair<uchar, ushort>& act = table[status.top() * (vts.size() + 1 + vns.size()) + idx];
            switch (act.first)
            {
            case 'S':
                if (!pParser->shift(tk.data))
                {
                    throw error<const char*>("shift error", __FILE__, __LINE__);
                    return false;
                }
                status.push(act.second);
                tokens.pop_front();
                idx = 0;
                break;
            case 'R':
                {
                    const Production& p = _rules[act.second];
                    if (!pParser->reduce(act.second))
                    {
                        throw error<const char*>("reduce error", __FILE__, __LINE__);
                        return false;
                    }
                    long j = getGoTo(status[p.right.size()], p.left);
                    if (j == -1)
                    {
                        throw error<const char*>("reduce error", __FILE__, __LINE__);
                        return false;
                    }
                    for (vector<Production::Item>::const_iterator k = p.right.begin(), o = p.right.end(); k != o; ++k)
                    {
                        status.pop();
                    }
                    status.push((ushort)j);
                    idx = 0;
                }
                break;
            case 'A':
                return true;
            default:
                ++idx;
                break;
            }
        }
        while (!status.empty())
        {
            const pair<uchar, ushort>& act = table[status.top() * (vts.size() + 1 + vns.size()) + vts.size()];
            switch (act.first)
            {
            case 'R':
                {
                    const Production& p = _rules[act.second];
                    if (!pParser->reduce(act.second))
                    {
                        throw error<const char*>("reduce error", __FILE__, __LINE__);
                        return false;
                    }
                    long j = getGoTo(status[p.right.size()], p.left);
                    if (j == -1)
                    {
                        throw error<const char*>("reduce error", __FILE__, __LINE__);
                        return false;
                    }
                    for (vector<Production::Item>::const_iterator k = p.right.begin(), o = p.right.end(); k != o; ++k)
                    {
                        status.pop();
                    }
                    status.push((ushort)j);
                }
                break;
            case 'A':
                return true;
            default:
                throw error<const char*>("some error with syntax", __FILE__, __LINE__);
                return false;
            }
        }
        return false;
    }

    long LALR1::index_of_vt(const string& str, long idx /* = 0 */)
    {
        for (size_t i = idx, m = vts.size(); i < m; ++i)
        {
            char* o = NULL;
            size_t sz = 0;
            if (vts[i].rule.parse(str.begin(), str.end(), o, sz) && sz == str.size()) return (long)i;
        }
        return -1;
    }

    long LALR1::getGoTo(ushort s, const Production::Item& i)
    {
        long j = index_of(vns.begin(), vns.end(), i);
        if (j == -1) return -1;
        return table[s * (vts.size() + 1 + vns.size()) + vts.size() + 1 + j].second;
    }
}
