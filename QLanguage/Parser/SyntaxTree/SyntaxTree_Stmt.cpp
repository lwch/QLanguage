/********************************************************************
	created:	2013/06/20
	created:	20:6:2013   22:12
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Stmt.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Stmt
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_Stmt.h"

namespace QLanguage
{
    SyntaxTree_Stmt::SyntaxTree_Stmt(SyntaxTree_Base* pStmt, Type type)
        : parent(sizeof(SyntaxTree_Stmt))
        , pStmt(pStmt)
        , _type(type)
    {
    }

    SyntaxTree_Stmt::~SyntaxTree_Stmt()
    {
    }

    void SyntaxTree_Stmt::print(ostream& stream, uint indent) const
    {
        this->printIndent(stream, indent);
        pStmt->print(stream, 0);
        switch (_type)
        {
        case Assign:
        case Call:
        case Declare:
        case Do:
            stream << ';';
            break;
        default:
            break;
        }
    }

    // stmt -> assign_desc ";"
    // stmt -> call_desc ";"
    // stmt -> declare_desc ";"
    // stmt -> do_desc ";"
    // stmt -> if_desc
    // stmt -> for_desc
    // stmt -> while_desc
    // stmt -> return_desc
    bool Parser::reduceStmt(ushort i)
    {
        SyntaxTree_Stmt* pStmt = allocator<SyntaxTree_Stmt>::allocate();
        switch (i)
        {
        case STMT_ASSIGN_DESC:                       // stmt -> assign_desc ";"
            shifts.pop();
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::Assign);
            break;
        case STMT_CALL_DESC:                         // stmt -> call_desc ";"
            shifts.pop();
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::Call);
            break;
        case STMT_DECLARE_DESC:                      // stmt -> declare_desc ";"
            shifts.pop();
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::Declare);
            break;
        case STMT_DO_DESC:                           // stmt -> do_desc ";"
            shifts.pop();
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::Do);
            break;
        case STMT_IF_DESC:                           // stmt -> if_desc
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::If);
            break;
        case STMT_FOR_DESC:                          // stmt -> for_desc
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::For);
            break;
        case STMT_WHILE_DESC:                        // stmt -> while_desc
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::While);
            break;
        case STMT_RETURN_DESC:                       // stmt -> return_desc
            construct(pStmt, syntaxTreeStack.top(), SyntaxTree_Stmt::Return);
            break;
        }
        
        context.data.insert(pStmt);
        
        syntaxTreeStack.pop();
        syntaxTreeStack.push(pStmt);
        
        return true;
    }
}
