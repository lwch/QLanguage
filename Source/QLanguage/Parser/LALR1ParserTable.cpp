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
//     void LALR1::output(const string& path)
//     {
//         time_t t;
//         time(&t);
//         char version = 1;
//         fstream stream(path, fstream::out | fstream::binary);
//         stream << PARSER_TABLE << version << (isX86() ? 86 : 64) << t << vts.size() << vns.size() << this->pStart->idx;
//         for (vector<pair<uchar, ushort> >::const_iterator i = table.begin(), m = table.end(); i != m; ++i)
//         {
//             stream << i->first << i->second;
//         }
//     }
// 
//     bool LALR1::parse(const list<Lexer::Token>& l)
//     {
//         stack<ushort> status;
//         status.push(pStart->idx);
//         list<Lexer::Token> tokens = l;
//         while (!tokens.empty())
//         {
//             Lexer::Token& tk = tokens.front();
//             long idx = index_of_vt(tk.data);
//             if (idx == -1)
//             {
//                 throw error<const char*>("get action error", __FILE__, __LINE__);
//                 return false;
//             }
//             const pair<uchar, ushort>& act = table[status.top() * (vts.size() + 1 + vns.size()) + idx];
//             switch (act.first)
//             {
//             case 'S':
//                 status.push(act.second);
//                 tokens.pop_front();
//                 break;
//             case 'R':
//                 {
//                     Production p = lr0.inputProductions[act.second];
//                     long j = getGoTo(status[p.right.size() + 1], p.left);
//                     if (j == -1)
//                     {
//                         throw error<const char*>("reduce error", __FILE__, __LINE__);
//                         return false;
//                     }
//                     for (vector<Production::Item>::const_iterator k = p.right.begin(), o = p.right.end(); k != o; ++k)
//                     {
//                         status.pop();
//                     }
//                     status.push((ushort)j);
//                 }
//                 break;
//             default:
//                 throw error<const char*>("some error with syntax", __FILE__, __LINE__);
//                 return false;
//             }
//         }
//         return true;
//     }
// 
//     long LALR1::index_of_vt(const string& str)
//     {
//         for (size_t i = 0, m = vts.size(); i < m; ++i)
//         {
//             char* o = NULL;
//             size_t sz = 0;
//             if (vts[i].rule.parse(str.begin(), str.end(), o, sz) && sz == str.size()) return i;
//         }
//         return -1;
//     }
// 
//     long LALR1::getGoTo(ushort s, const Production::Item& i)
//     {
//         long j = index_of(vns.begin(), vns.end(), i);
//         if (j == -1) return -1;
//         return table[s * (vts.size() + 1 + vns.size()) + j].second;
//     }
}
