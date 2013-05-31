/********************************************************************
	created:	2013/05/31
	created:	31:5:2013   15:03
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Base.h
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Base
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_SYNTAX_TREE_BASE_H_
#define _QLANGUAGE_SYNTAX_TREE_BASE_H_

#include "../../../QCore/Library/ostream.h"
#include "../../../QCore/Library/vector.h"

namespace QLanguage
{
    using namespace Library;

    class SyntaxTree_Base
    {
        enum { indent = 2 };
    public:
        SyntaxTree_Base(uint size);
        virtual ~SyntaxTree_Base();

        virtual void print(ostream&, uint)const=0;

        inline const uint size()const { return _size; }

        inline void setNext(SyntaxTree_Base* next)
        {
            this->next = next;
        }

        inline void pushChild(SyntaxTree_Base* child)
        {
            childs.push_back(child);
        }
    protected:
        uint                     _size;
        vector<SyntaxTree_Base*> childs;
        SyntaxTree_Base*         next;
    };
}

#endif
