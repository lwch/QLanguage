/********************************************************************
	created:	2013/05/16
	created:	16:5:2013   13:52
	filename: 	\QLanguage\Parser\Production.cpp
	file path:	\QLanguage\Parser
	file base:	Production
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "Production.h"
#include "LRProduction.h"

namespace QLanguage
{
    // Production::Item
    Production::Item::Item() : type(NoTerminalSymbol), index(inc())
    {
    }

    Production::Item::Item(Rule::Context* pContext) : type(TerminalSymbol), rule(pContext), index(0)
    {
    }

    Production::Item::Item(const string& name) : type(NoTerminalSymbol), index(inc()), name(name)
    {
    }

    Production::Item::Item(const Rule& rule) : type(TerminalSymbol), rule(rule), index(inc())
    {
    }

    Production::Item::Item(const Item& i) : type(i.type) , index(i.index) , name(i.name)
    {
        if (type == TerminalSymbol) rule = i.rule;
    }

    Production::Item& Production::Item::operator=(const Item& i)
    {
        if (&i != this)
        {
            type  = i.type;
            rule  = i.rule;
            index = i.index;
            name  = i.name;
        }
        return *this;
    }

    uint Production::Item::inc()
    {
        static uint i = 0;
        return i++;
    }

    bool Production::Item::loadFromData(const char*& data)
    {
        index = *reinterpret_cast<const uint*>(data);
        data += sizeof(uint);
        type = (Type)*data;
        ++data;
        return type == TerminalSymbol ? rule.loadFromData(data) : true;
    }

    bool Production::Item::loadFromStream(istream& stream)
    {
        stream >> index;
        stream >> (uchar&)type;
        return type == TerminalSymbol ? rule.loadFromStream(stream) : true;
    }

    const bool Production::Item::output(ostream& stream)const
    {
        stream << index;
        stream << (uchar)type;
        return type == TerminalSymbol ? rule.output(stream) : true;
    }
    // Production
    Production::Production()
    {
    }

    Production::Production(const Item& left) : left(left), index(inc())
    {
    }

    Production::Production(const Item& left, const Item& item) : left(left), index(inc())
    {
        right.push_back(item);
    }

    Production::Production(const Item& left, const vector<Item>& right) : left(left), right(right), index(inc())
    {
    }

    Production::Production(const Production& p) : left(p.left), right(p.right), index(p.index)
    {
    }

    const bool Production::operator==(const LALR1Production& p)const
    {
        return left == p.left && right == p.right;
    }

    void Production::print(Library::ostream& stream)const
    {
        stream << string::format("%s ->", left.name.c_str());
        for (vector<Item>::const_iterator i = right.begin(), m = right.end(); i != m; ++i)
        {
            if (i->type == Item::TerminalSymbol)
            {
                stream << " ";
                i->rule.printShowName(stream);
            }
            else
            {
                stream << string::format(" %s", i->name.c_str());
            }
        }
        stream << endl;
    }

    bool Production::loadFromData(const char*& data, Rule::Context* pContext)
    {
        index = *reinterpret_cast<const uint*>(data);
        data += sizeof(uint);
        if (!left.loadFromData(data)) return false;
        size_t size = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        right.reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            Production::Item item(pContext);
            if (!item.loadFromData(data)) return false;
            right.push_back(item);
        }
        return true;
    }

    bool Production::loadFromStream(istream& stream, Rule::Context* pContext)
    {
        stream >> index;
        if (!left.loadFromStream(stream)) return false;
        size_t size;
        stream >> size;
        right.reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            Production::Item item(pContext);
            if (!item.loadFromStream(stream)) return false;
            right.push_back(item);
        }
        return true;
    }

    const bool Production::output(Library::ostream& stream)const
    {
        stream << index;
        if (!left.output(stream)) return false;
        stream << right.size();
        for (vector<Production::Item>::const_iterator i = right.begin(), m = right.end(); i != m; ++i)
        {
            if (!i->output(stream)) return false;
        }
        return true;
    }

    uint Production::inc()
    {
        static uint i = 0;
        return i++;
    }
}