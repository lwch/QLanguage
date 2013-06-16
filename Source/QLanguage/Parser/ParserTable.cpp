/********************************************************************
	created:	2013/05/06
	created:	6:5:2013   22:58
	filename: 	\QLanguage\Parser\ParserTable.cpp
	file path:	\QLanguage\Parser
	file base:	ParserTable
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "ParserTable.h"

#define PARSER_TABLE "QPARSERGENERATOR PARSERTABLE"

namespace QLanguage
{
    ParserTable::ParserTable()
    {
    }

    bool ParserTable::loadFromData(const char* data, size_t size)
    {
        if (compareString(data, size, PARSER_TABLE)) data += strlen(PARSER_TABLE);
        //uchar version = *data;
        ++data;
        data += sizeof(time_t);
        size_t vtCount = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        size_t vnCount = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        size_t rulesCount = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        size_t itemCount = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        iStart = *reinterpret_cast<const uint*>(data);
        data += sizeof(uint);

        vts.reserve(vtCount);
        vns.reserve(vnCount);
        rules.reserve(rulesCount);
        items.reserve(itemCount);
        table.reserve(itemCount * (vtCount + 1 + vnCount));

        for (size_t i = 0; i < itemCount * (vtCount + 1 + vnCount); ++i)
        {
            pair<uchar, ushort> p;
            p.first = *data;
            ++data;
            p.second = *reinterpret_cast<const ushort*>(data);
            data += sizeof(ushort);
            table.push_back(p);
        }

        for (size_t i = 0; i < vtCount; ++i)
        {
            Production::Item item(&ruleContext);
            item.loadFromData(data);
            vts.push_back(item);
        }

        for (size_t i = 0; i < vnCount; ++i)
        {
            Production::Item item;
            item.loadFromData(data);
            vns.push_back(item);
        }

        for (size_t i = 0; i < rulesCount; ++i)
        {
            Production p;
            p.loadFromData(data, &ruleContext);
            rules.push_back(p);
        }

        for (size_t i = 0; i < itemCount; ++i)
        {
            items.push_back(*reinterpret_cast<const uint*>(data));
            data += sizeof(uint);
        }
        return true;
    }

    const bool ParserTable::compareString(const char* data, size_t size, const char* compare)const
    {
        size_t sz = strlen(compare);
        if (size < sz) return false;
        for (size_t i = 0; i < sz; ++i)
        {
            if (data[i] != compare[i]) return false;
        }
        return true;
    }

    bool ParserTable::parse(const list<Lexer::Token>& l, BasicParser* pParser)
    {
        stack<ushort> status;
        status.push(iStart);
        list<Lexer::Token> tokens = l;
        long idx = (long)vts.size() - 1;
        while (!tokens.empty())
        {
            Lexer::Token& tk = tokens.front();
            idx = index_of_vt(tk.data, idx);
            if (idx == -1)
            {
                throw error<const char*>("get action error", __FILE__, __LINE__);
                return false;
            }
            const pair<uchar, ushort>& act = table[index_of(items.begin(), items.end(), status.top()) * (vts.size() + 1 + vns.size()) + idx];
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
                idx = (long)vts.size() - 1;
                break;
            case 'R':
                {
                    const Production& p = rules[act.second];
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
                    idx = (long)vts.size() - 1;
                }
                break;
            case 'A':
                return true;
            default:
                if (idx == 0)
                {
                    throw error<const char*>("get action error", __FILE__, __LINE__);
                    return false;
                }
                --idx;
                break;
            }
        }
        while (!status.empty())
        {
            const pair<uchar, ushort>& act = table[index_of(items.begin(), items.end(), status.top()) * (vts.size() + 1 + vns.size()) + vts.size()];
            switch (act.first)
            {
            case 'R':
                {
                    const Production& p = rules[act.second];
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
                return pParser->reduce(0);
            default:
                throw error<const char*>("some error with syntax", __FILE__, __LINE__);
                return false;
            }
        }
        return false;
    }

    long ParserTable::index_of_vt(const string& str, long idx /* = 0 */)
    {
        for (long i = idx; i >= 0; --i)
        {
            char* o = NULL;
            size_t sz = 0;
            if (vts[i].rule.parse(str.begin(), str.end(), o, sz) && sz == str.size()) return (long)i;
        }
        return -1;
    }

    long ParserTable::getGoTo(ushort s, const Production::Item& i)
    {
        long j = index_of(vns.begin(), vns.end(), i);
        if (j == -1) return -1;
        return table[index_of(items.begin(), items.end(), s) * (vts.size() + 1 + vns.size()) + vts.size() + 1 + j].second;
    }
}
