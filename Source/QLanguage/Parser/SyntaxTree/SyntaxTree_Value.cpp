/********************************************************************
	created:	2013/06/01
	created:	1:6:2013   20:08
	filename: 	\QLanguage\Parser\SyntaxTree\SyntaxTree_Value.cpp
	file path:	\QLanguage\Parser\SyntaxTree
	file base:	SyntaxTree_Value
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include "../Parser.h"
#include "SyntaxTree_MemberList.h"
#include "SyntaxTree_ArrayValue.h"
#include "SyntaxTree_Value.h"

namespace QLanguage
{
    SyntaxTree_Value::SyntaxTree_Value(const string& value, Type type)
        : parent(sizeof(SyntaxTree_Value))
        , value(value)
        , _type(type)
        , pContent(NULL)
    {
    }

    SyntaxTree_Value::SyntaxTree_Value(SyntaxTree_Base* pContent)
        : parent(sizeof(SyntaxTree_Value))
        , _type(Member)
        , pContent(pContent)
    {
    }

    SyntaxTree_Value::~SyntaxTree_Value()
    {
    }

    void SyntaxTree_Value::print(ostream& stream, uint indent)const
    {
        if (pContent) pContent->print(stream, indent);
        else stream << value;
    }

    const HASH_KEY_TYPE SyntaxTree_Value::hash()const
    {
        if (_type != Member) return ::hash<string>()(value);
        return 0; // TODO: hash for call function
    }

    const bool SyntaxTree_Value::greaterEqual(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return value == "true" ? true : x.value == "false";
            case Real:
                return value == "true" ? 1 >= atof(x.value.c_str()) : 0 >= atof(x.value.c_str());
            case Digit:
                return value == "true" ? 1 >= atol(x.value.c_str()) : 0 >= atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed boolean to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atof(value.c_str()) >= 1 : atof(value.c_str()) >= 0;
            case Real:
                return atof(value.c_str()) >= atof(x.value.c_str());
            case Digit:
                return atof(value.c_str()) >= atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed real to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atol(value.c_str()) >= 1 : atol(value.c_str()) >= 0;
            case Real:
                return atol(value.c_str()) >= atof(x.value.c_str());
            case Digit:
                return atol(value.c_str()) >= atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed digit to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case String:
            if (x._type == String) return true; // TODO
            else
            {
                throw error<const char*>("Can't compare from value typed string to number", __FILE__, __LINE__);
                return false;
            }
            break;
        case Member: // TODO
            return true;
        default:
            return false;
        }
    }

    const bool SyntaxTree_Value::lessEqual(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return value == "true" ? x.value == "false" : true;
            case Real:
                return value == "true" ? 1 <= atof(x.value.c_str()) : 0 <= atof(x.value.c_str());
            case Digit:
                return value == "true" ? 1 <= atol(x.value.c_str()) : 0 <= atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed boolean to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atof(value.c_str()) <= 1 : atof(value.c_str()) <= 0;
            case Real:
                return atof(value.c_str()) <= atof(x.value.c_str());
            case Digit:
                return atof(value.c_str()) <= atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed real to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atol(value.c_str()) <= 1 : atol(value.c_str()) <= 0;
            case Real:
                return atol(value.c_str()) <= atof(x.value.c_str());
            case Digit:
                return atol(value.c_str()) <= atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed digit to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case String:
            if (x._type == String) return true; // TODO
            else
            {
                throw error<const char*>("Can't compare from value typed string to number", __FILE__, __LINE__);
                return false;
            }
            break;
        case Member: // TODO
            return true;
        default:
            return false;
        }
    }

    const bool SyntaxTree_Value::equal(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return value == x.value;
            case Real:
                return value == "true" ? 1 == atof(x.value.c_str()) : 0 == atof(x.value.c_str());
            case Digit:
                return value == "true" ? 1 == atol(x.value.c_str()) : 0 == atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed boolean to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atof(value.c_str()) == 1 : atof(value.c_str()) == 0;
            case Real:
                return atof(value.c_str()) == atof(x.value.c_str());
            case Digit:
                return atof(value.c_str()) == atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed real to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atol(value.c_str()) == 1 : atol(value.c_str()) == 0;
            case Real:
                return atol(value.c_str()) == atof(x.value.c_str());
            case Digit:
                return atol(value.c_str()) == atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed digit to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case String:
            if (x._type == String) return value == x.value; // TODO
            else
            {
                throw error<const char*>("Can't compare from value typed string to number", __FILE__, __LINE__);
                return false;
            }
            break;
        case Member: // TODO
            return true;
        default:
            return false;
        }
    }

    const bool SyntaxTree_Value::greater(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return value == "true" ? x.value == "false" : false;
            case Real:
                return value == "true" ? 1 > atof(x.value.c_str()) : 0 > atof(x.value.c_str());
            case Digit:
                return value == "true" ? 1 > atol(x.value.c_str()) : 0 > atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed boolean to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atof(value.c_str()) > 1 : atof(value.c_str()) > 0;
            case Real:
                return atof(value.c_str()) > atof(x.value.c_str());
            case Digit:
                return atof(value.c_str()) > atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed real to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atol(value.c_str()) > 1 : atol(value.c_str()) > 0;
            case Real:
                return atol(value.c_str()) > atof(x.value.c_str());
            case Digit:
                return atol(value.c_str()) > atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed digit to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case String:
            if (x._type == String) return true; // TODO
            else
            {
                throw error<const char*>("Can't compare from value typed string to number", __FILE__, __LINE__);
                return false;
            }
            break;
        case Member: // TODO
            return true;
        default:
            return false;
        }
    }

    const bool SyntaxTree_Value::less(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return value == "true" ? false : x.value == "true";
            case Real:
                return value == "true" ? 1 < atof(x.value.c_str()) : 0 < atof(x.value.c_str());
            case Digit:
                return value == "true" ? 1 < atol(x.value.c_str()) : 0 < atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed boolean to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atof(value.c_str()) < 1 : atof(value.c_str()) < 0;
            case Real:
                return atof(value.c_str()) < atof(x.value.c_str());
            case Digit:
                return atof(value.c_str()) < atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed real to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return x.value == "true" ? atol(value.c_str()) < 1 : atol(value.c_str()) < 0;
            case Real:
                return atol(value.c_str()) < atof(x.value.c_str());
            case Digit:
                return atol(value.c_str()) < atol(x.value.c_str());
            case String:
                throw error<const char*>("Can't compare from value typed digit to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return true;
            default:
                return false;
            }
            break;
        case String:
            if (x._type == String) return true; // TODO
            else
            {
                throw error<const char*>("Can't compare from value typed string to number", __FILE__, __LINE__);
                return false;
            }
            break;
        case Member: // TODO
            return true;
        default:
            return false;
        }
    }

    // value_desc -> "true"
    // value_desc -> "false"
    // value_desc -> "{Real}"
    // value_desc -> "{Digit}"
    // value_desc -> "{String}"
    bool Parser::reduceValueNormal(ushort i)
    {
        SyntaxTree_Value* pValue = allocator<SyntaxTree_Value>::allocate();
        switch (i)
        {
        case VALUE_DESC_TRUE:   // value_desc -> "true"
        case VALUE_DESC_FALSE:  // value_desc -> "false"
            construct(pValue, shifts.top(), SyntaxTree_Value::Boolean);
            break;
        case VALUE_DESC_REAL:   // value_desc -> "{Real}"
            construct(pValue, shifts.top(), SyntaxTree_Value::Real);
            break;
        case VALUE_DESC_DIGIT:  // value_desc -> "{Digit}"
            construct(pValue, shifts.top(), SyntaxTree_Value::Digit);
            break;
        case VALUE_DESC_STRING: // value_desc -> "{String}"
            construct(pValue, shifts.top(), SyntaxTree_Value::String);
            break;
        }

        context.data.insert(pValue);

        syntaxTreeStack.push(pValue);

        shifts.pop();

        return true;
    }

    // value_desc -> member_desc
    // value_desc -> values
    bool Parser::reduceValueContent(ushort i)
    {
#ifdef _DEBUG
        if (i == VALUE_DESC_MEMBER_DESC) TRY_CAST(SyntaxTree_MemberList*, syntaxTreeStack.top());
        else TRY_CAST(SyntaxTree_ArrayValue*, syntaxTreeStack.top());
#endif
        SyntaxTree_Value* pValue = allocator<SyntaxTree_Value>::allocate();
        construct(pValue, syntaxTreeStack.top());

        context.data.insert(pValue);

        syntaxTreeStack.pop();
        syntaxTreeStack.push(pValue);

        return true;
    }
}
