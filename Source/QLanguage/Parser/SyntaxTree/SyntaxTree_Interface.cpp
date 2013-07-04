/********************************************************************
	created:	2013/06/07
	created:	7:6:2013   11:27
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Interface.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Interface
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_InterfaceContentList.h"
#include "SyntaxTree_Interface.h"

namespace QLanguage
{
    SyntaxTree_Interface::SyntaxTree_Interface(const string& name, SyntaxTree_InterfaceContentList* pContentList)
        : parent(sizeof(SyntaxTree_Interface))
        , name(name)
        , pContentList(pContentList)
    {
    }

    SyntaxTree_Interface::SyntaxTree_Interface(const string& name)
        : parent(sizeof(SyntaxTree_Interface))
        , name(name)
        , pContentList(NULL)
    {
    }

    SyntaxTree_Interface::~SyntaxTree_Interface()
    {
    }

    void SyntaxTree_Interface::print(ostream& stream, uint indent)const
    {
        this->printIndent(stream, indent);
        stream << "interface " << name << " {" << endl;
        if (pContentList) pContentList->print(stream, indent);
        stream << '}' << endl;
    }

    // interface_desc -> "interface" "{Letter}" "{" interface_content "}"
    bool Parser::reduceInterfaceWidthContent()
    {
#ifdef _DEBUG
        TRY_CAST(SyntaxTree_InterfaceContentList*, syntaxTreeStack.top());
#endif
        shifts.pop();
        shifts.pop();

        SyntaxTree_Interface* pInterface = allocator<SyntaxTree_Interface>::allocate();
        construct(pInterface, shifts.top(), dynamic_cast<SyntaxTree_InterfaceContentList*>(syntaxTreeStack.top()));

        context.data.insert(pInterface);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pInterface);

        shifts.pop();
        shifts.pop();

        return true;
    }

    // interface_desc -> "interface" "{Letter}" "{" "}"
    bool Parser::reduceInterfaceNoContent()
    {
        shifts.pop();
        shifts.pop();

        SyntaxTree_Interface* pInterface = allocator<SyntaxTree_Interface>::allocate();
        construct(pInterface, shifts.top());

        context.data.insert(pInterface);

        syntaxTreeStack.push(pInterface);

        shifts.pop();
        shifts.pop();

        return true;
    }
}
