/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   20:35
	filename: 	\QLanguage\Parser\Parser.h
	file path:	\QLanguage\Parser
	file base:	Parser
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_PARSER_H_
#define _QLANGUAGE_PARSER_H_

#include "../../QCore/Library/hashset.h"

#include "SyntaxTree/SyntaxTree_Type.h"
#include "BasicParser.h"

namespace QLanguage
{
#define TYPE_DESC_UNSIGNED_CHAR                      1   // type_desc -> "unsigned" "char"
#define TYPE_DESC_CHAR                               2   // type_desc -> "char"
#define TYPE_DESC_UNSIGNED_SHORT                     3   // type_desc -> "unsigned" "short"
#define TYPE_DESC_SHORT                              4   // type_desc -> "short"
#define TYPE_DESC_UNSIGNED_INT                       5   // type_desc -> "unsigned" "int"
#define TYPE_DESC_INT                                6   // type_desc -> "int"
#define TYPE_DESC_UNSIGNED_LONG                      7   // type_desc -> "unsigned" "long"
#define TYPE_DESC_LONG                               8   // type_desc -> "long"
#define TYPE_DESC_FLOAT                              9   // type_desc -> "float"
#define TYPE_DESC_DOUBLE                             10  // type_desc -> "double"
#define TYPE_DESC_STRING                             11  // type_desc -> "string"
#define TYPE_DESC_AUTO                               12  // type_desc -> "auto"
#define TYPE_DESC_LETTER                             13  // type_desc -> "{Letter}"
#define GLOBAL_FUNCTION_DESC_TYPE_DESC_NOPARAM_BLOCK 63  // global_function_desc -> type_desc "{Letter}" "(" ")" block
#define STMT_DECLARE                                 116 // stmt -> declare_desc ";"
#define DECLARE_DESC_DECLARE_DESC_LETTER             125 // declare_desc -> declare_desc "," "{Letter}"
#define DECLARE_DESC_LETTER                          129 // declare_desc -> type_desc "{Letter}"

    class Parser : public BasicParser
    {
    public:
        Parser(const vector<Production>& productions);
        virtual ~Parser();

        virtual bool shift(const string& s);
        virtual bool reduce(ushort i);
    protected:
        bool reduceType1Size(ushort i);
        bool reduceType2Size(ushort i);
        bool reduceGlobalFunction4();
        bool reduceDeclare48(ushort i);

        inline bool pop1Shifts()
        {
            shifts.pop();
            return true;
        }

        inline bool pop2Shifts()
        {
            shifts.pop();
            shifts.pop();
            return true;
        }
    protected:
        struct
        {
            hashset<SyntaxTree_Base*> data;
        }context;
        stack<SyntaxTree_Base*>  syntaxTreeStack;
        stack<string>            shifts;
    };
}

#endif
