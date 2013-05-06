/********************************************************************
	created:	2013/05/06
	created:	6:5:2013   22:58
	filename: 	E:\QLanguage\Source\Source\Calculator\Parser\ParserTable.cpp
	file path:	E:\QLanguage\Source\Source\Calculator\Parser
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
        uchar version = *data;
        ++data;
        uint timeSize = *reinterpret_cast<const uint*>(data);
        data += sizeof(uint);
        data += timeSize;
        size_t vtCount = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        size_t vnCount = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        size_t itemCount = *reinterpret_cast<const size_t*>(data);
        data += sizeof(size_t);
        uint startIndex = *reinterpret_cast<const uint*>(data);
        data += sizeof(uint);

        vts.reserve(vtCount);
        vns.reserve(vnCount);
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
}
