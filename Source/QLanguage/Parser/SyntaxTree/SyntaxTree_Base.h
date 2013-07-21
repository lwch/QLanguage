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
#include "../../../QCore/Library/hash.h"

namespace QLanguage
{

class SyntaxTree_Do;
    using namespace Library;

#define TRY_CAST(type, value) \
    do \
    {  \
        if (static_cast<type>(value) == NULL) throw error<string>(string::format("can't cast to type: %s", #type), __FILE__, __LINE__);  \
    } while (0)

    class Parser;

    class SyntaxTree_Base
    {
        enum { indent = 4 };

        friend class SyntaxTree_ArrayLst;
        friend class SyntaxTree_Block;
        friend class SyntaxTree_CallList;
        friend class SyntaxTree_ClassContentList;
        friend class SyntaxTree_DeclareList;
        friend class SyntaxTree_Do;
        friend class SyntaxTree_Else;
        friend class SyntaxTree_For;
        friend class SyntaxTree_Function;
        friend class SyntaxTree_GlobalFunction;
        friend class SyntaxTree_If;
        friend class SyntaxTree_InterfaceContentList;
        friend class SyntaxTree_ItemList;
        friend class SyntaxTree_MemberList;
        friend class SyntaxTree_ParamterList;
        friend class SyntaxTree_Return;
        friend class SyntaxTree_StmtList;
        friend class SyntaxTree_SwitchContent;
        friend class SyntaxTree_SwitchContentList;
        friend class SyntaxTree_TemplateList;
        friend class SyntaxTree_ValueList;
        friend class SyntaxTree_ValuesList;
        friend class SyntaxTree_While;
    public:
        SyntaxTree_Base(uint size);
        virtual ~SyntaxTree_Base();

        virtual void print(ostream&, uint)const=0;
        virtual string type()const=0;

        inline const uint size()const { return _size; }
        
        inline void printIndent(ostream& stream, uint indent)const
        {
            for (uint i = 0; i < indent; ++i)
            {
                stream << ' ';
            }
        }
        
        inline void printChilds(ostream& stream, uint indent)const
        {
            for (vector<SyntaxTree_Base*>::const_iterator i = childs.begin(), m = childs.end(); i != m; ++i)
            {
                (*i)->print(stream, indent);
            }
        }

        inline void pushChild(SyntaxTree_Base* child)
        {
            childs.push_back(child);
        }

        template <typename T1, typename T2>
        inline static const bool checkEqual(T1* p1, T2* p2)
        {
            return (p1 && p2 && *p1 == *p2) ||
                   (p1 == NULL && p2 == NULL);
        }

        template <typename T1, typename T2>
        inline static const bool checkNotEqual(T1* p1, T2* p2)
        {
            return (p1 && (p2 == NULL || *p1 != *p2)) ||
                   (p1 == NULL && p2);
        }

        inline virtual bool make(Parser* pParser) { return true; }

        inline virtual const HASH_KEY_TYPE hash()const { return 0; }

        inline virtual const short getRegister(Parser* pParser)const { return -1; }

        inline virtual const bool isValue()const { return false; }
        inline virtual const bool isConstValue()const { return false; }

        virtual const bool operator==(const SyntaxTree_Base& x)const=0;
        virtual const bool operator!=(const SyntaxTree_Base& x)const=0;
    protected:
        uint                     _size;
        vector<SyntaxTree_Base*> childs;
    };
}

#endif
