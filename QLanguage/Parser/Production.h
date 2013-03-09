/********************************************************************
	created:	2013/03/09
	created:	9:3:2013   14:28
	filename: 	\QLanguage\Parser\Production.h
	file path:	\QLanguage\Parser
	file base:	Production
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_PRODUCTION_H_
#define _QLANGUAGE_PRODUCTION_H_

#include <assert.h>

#include "../../QCore/Library/list.h"

using namespace QLanguage::Library;

namespace QLanguage
{
    struct Production
    {
    public:
        struct Item
        {
            enum Type
            {
                TerminalSymbol,
                NoTerminalSymbol,
            }type;

            union
            {
                struct
                {
                    char value;
                }Char;

                struct
                {
                    char* value;
                }String;
            }data;

            Item(char c) : type(TerminalSymbol)
            {
                data.Char.value = c;
            }

            Item(char* s, Type t) : type(t)
            {
                data.String.value = s;
            }
        };
    public:
        Production(const Item& left, const list<Item>& right) : left(left), right(right) {}
    public:
        Item left;
        list<Item> right;
    };
}

#endif
