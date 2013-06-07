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

#define TRY_CAST(type, value) if (static_cast<type>(value) == NULL) throw error<string>(string::format("can't cast to type: %s", #type), __FILE__, __LINE__)

    class SyntaxTree_Base
    {
        enum { indent = 2 };

        friend class SyntaxTree_ItemList;
        friend class SyntaxTree_MemberList;
        friend class SyntaxTree_StmtList;
        friend class SyntaxTree_ValueList;
        friend class SyntaxTree_ParamterList;
        friend class SyntaxTree_InterfaceContentList;
        friend class SyntaxTree_ClassContentList;
    public:
        SyntaxTree_Base(uint size);
        virtual ~SyntaxTree_Base();

        virtual void print(ostream&, uint)const=0;

        inline const uint size()const { return _size; }

        inline void pushChild(SyntaxTree_Base* child)
        {
            childs.push_back(child);
        }

        virtual const bool operator==(const SyntaxTree_Base& x)const=0;
        virtual const bool operator!=(const SyntaxTree_Base& x)const=0;
    protected:
        uint                     _size;
        vector<SyntaxTree_Base*> childs;
    };
}

#endif
