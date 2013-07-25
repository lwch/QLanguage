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

    const bool SyntaxTree_Value::operator>=(const SyntaxTree_Value& x)const
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
                return value == "true" ? 1 >= atoi(x.value.c_str()) : 0 >= atoi(x.value.c_str());
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
                return atof(value.c_str()) >= atoi(x.value.c_str());
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
                return x.value == "true" ? atoi(value.c_str()) >= 1 : atoi(value.c_str()) >= 0;
            case Real:
                return atoi(value.c_str()) >= atof(x.value.c_str());
            case Digit:
                return atoi(value.c_str()) >= atoi(x.value.c_str());
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

    const bool SyntaxTree_Value::operator<=(const SyntaxTree_Value& x)const
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
                return value == "true" ? 1 <= atoi(x.value.c_str()) : 0 <= atoi(x.value.c_str());
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
                return atof(value.c_str()) <= atoi(x.value.c_str());
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
                return x.value == "true" ? atoi(value.c_str()) <= 1 : atoi(value.c_str()) <= 0;
            case Real:
                return atoi(value.c_str()) <= atof(x.value.c_str());
            case Digit:
                return atoi(value.c_str()) <= atoi(x.value.c_str());
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
                return value == "true" ? 1 == atoi(x.value.c_str()) : 0 == atoi(x.value.c_str());
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
                return atof(value.c_str()) == atoi(x.value.c_str());
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
                return x.value == "true" ? atoi(value.c_str()) == 1 : atoi(value.c_str()) == 0;
            case Real:
                return atoi(value.c_str()) == atof(x.value.c_str());
            case Digit:
                return atoi(value.c_str()) == atoi(x.value.c_str());
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

    const bool SyntaxTree_Value::operator>(const SyntaxTree_Value& x)const
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
                return value == "true" ? 1 > atoi(x.value.c_str()) : 0 > atoi(x.value.c_str());
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
                return atof(value.c_str()) > atoi(x.value.c_str());
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
                return x.value == "true" ? atoi(value.c_str()) > 1 : atoi(value.c_str()) > 0;
            case Real:
                return atoi(value.c_str()) > atof(x.value.c_str());
            case Digit:
                return atoi(value.c_str()) > atoi(x.value.c_str());
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

    const bool SyntaxTree_Value::operator<(const SyntaxTree_Value& x)const
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
                return value == "true" ? 1 < atoi(x.value.c_str()) : 0 < atoi(x.value.c_str());
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
                return atof(value.c_str()) < atoi(x.value.c_str());
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
                return x.value == "true" ? atoi(value.c_str()) < 1 : atoi(value.c_str()) < 0;
            case Real:
                return atoi(value.c_str()) < atof(x.value.c_str());
            case Digit:
                return atoi(value.c_str()) < atoi(x.value.c_str());
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

    const bool SyntaxTree_Value::operator&&(const SyntaxTree_Value &x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return value == "true" && x.value == "true";
            case Real:
                return value == "true" && atof(x.value.c_str());
            case Digit:
                return value == "true" && atoi(x.value.c_str());
            case String:
                throw error<const char*>("Can't logicand from value typed bool to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return false;
            default:
                return false;
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return atof(value.c_str()) && x.value == "true";
            case Real:
                return atof(value.c_str()) && atof(x.value.c_str());
            case Digit:
                return atof(value.c_str()) && atoi(x.value.c_str());
            case String:
                throw error<const char*>("Can't logicand from value typed real to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return false;
            default:
                return false;
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return atoi(value.c_str()) && x.value == "true";
            case Real:
                return atoi(value.c_str()) && atof(x.value.c_str());
            case Digit:
                return atoi(value.c_str()) && atoi(x.value.c_str());
            case String:
                throw error<const char*>("Can't logicand from value typed digit to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return false;
            default:
                return false;
            }
            break;
        case String:
            throw error<const char*>("Can't logicand from value typed string to other type", __FILE__, __LINE__);
            return false;
        case Member: // TODO
            return false;
        default:
            return false;
        }
    }

    const bool SyntaxTree_Value::operator||(const SyntaxTree_Value &x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return value == "true" || x.value == "true";
            case Real:
                return value == "true" || atof(x.value.c_str());
            case Digit:
                return value == "true" || atoi(x.value.c_str());
            case String:
                throw error<const char*>("Can't logicand from value typed bool to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return false;
            default:
                return false;
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return atof(value.c_str()) || x.value == "true";
            case Real:
                return atof(value.c_str()) || atof(x.value.c_str());
            case Digit:
                return atof(value.c_str()) || atoi(x.value.c_str());
            case String:
                throw error<const char*>("Can't logicand from value typed real to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return false;
            default:
                return false;
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return atoi(value.c_str()) || x.value == "true";
            case Real:
                return atoi(value.c_str()) || atof(x.value.c_str());
            case Digit:
                return atoi(value.c_str()) || atoi(x.value.c_str());
            case String:
                throw error<const char*>("Can't logicand from value typed digit to string", __FILE__, __LINE__);
                return false;
            case Member: // TODO
                return false;
            default:
                return false;
            }
            break;
        case String:
            throw error<const char*>("Can't logicand from value typed string to other type", __FILE__, __LINE__);
            return false;
        case Member: // TODO
            return false;
        default:
            return false;
        }
    }

    VM::Variant SyntaxTree_Value::operator&(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(value == "true" && x.value == "true" ? 1 : 0);
            case Real:
                throw error<const char*>("Can't bitand from value typed bool to real", __FILE__, __LINE__);
                return VM::Variant();
            case Digit:
                return VM::Variant(value == "true" ? 1 & atoi(x.value.c_str()) : 0);
            case String:
                throw error<const char*>("Can't bitand from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            throw error<const char*>("Can't bitand from value typed real to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(x.value == "true" ? atoi(value.c_str()) & 1 : 0);
            case Real:
                throw error<const char*>("Can't bitand from value typed digit to real", __FILE__, __LINE__);
                return VM::Variant();
            case Digit:
                return VM::Variant(atoi(value.c_str()) & atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't bitand from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            throw error<const char*>("Can't bitand from value typed string to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::operator|(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(value == "false" && x.value == "false" ? 0 : 1);
            case Real:
                throw error<const char*>("Can't bitand from value typed bool to real", __FILE__, __LINE__);
                return VM::Variant();
            case Digit:
                return VM::Variant(value == "false" ? atoi(x.value.c_str()) : 1 | atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't bitand from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            throw error<const char*>("Can't bitand from value typed real to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(x.value == "false" ? atoi(value.c_str()) : atoi(value.c_str()) | 1);
            case Real:
                throw error<const char*>("Can't bitand from value typed digit to real", __FILE__, __LINE__);
                return VM::Variant();
            case Digit:
                return VM::Variant(atoi(value.c_str()) | atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't bitand from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            throw error<const char*>("Can't bitand from value typed string to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::operator^(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(value == x.value ? (value == "true" ? 1 : 0) : 1);
            case Real:
                throw error<const char*>("Can't bitand from value typed bool to real", __FILE__, __LINE__);
                return VM::Variant();
            case Digit:
                return VM::Variant(value == "true" ? 1 ^ atoi(x.value.c_str()) : atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't bitand from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            throw error<const char*>("Can't bitand from value typed real to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(x.value == "false" ? atoi(value.c_str()) : atoi(value.c_str()) ^ 1);
            case Real:
                throw error<const char*>("Can't bitand from value typed digit to real", __FILE__, __LINE__);
                return VM::Variant();
            case Digit:
                return VM::Variant(atoi(value.c_str()) & atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't bitand from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            throw error<const char*>("Can't bitand from value typed string to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::operator+(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(value == "true" ? 1 + (x.value == "true" ? 1 : 0) : x.value == "true" ? 1 : 0);
            case Real:
                return VM::Variant(value == "true" ? 1 + atof(x.value.c_str()) : atof(x.value.c_str()));
            case Digit:
                return VM::Variant(value == "true" ? 1 + atoi(x.value.c_str()) : atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't add from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(atof(value.c_str()) + (x.value == "true" ? 1 : 0));
            case Real:
                return VM::Variant(atof(value.c_str()) + atof(value.c_str()));
            case Digit:
                return VM::Variant(atof(value.c_str()) + atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't add from value typed real to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(atoi(value.c_str()) + (x.value == "true" ? 1 : 0));
            case Real:
                return VM::Variant(atoi(value.c_str()) + atof(x.value.c_str()));
            case Digit:
                return VM::Variant(atoi(value.c_str()) + atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't add from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            if (x._type == String) return VM::Variant(value + x.value);
            else
            {
                throw error<const char*>("Can't add from value typed string to number", __FILE__, __LINE__);
                return VM::Variant();
            }
            break;
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::operator-(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(value == "true" ? 1 - (x.value == "true" ? 1 : 0) : x.value == "true" ? -1 : 0);
            case Real:
                return VM::Variant(value == "true" ? 1 - atof(x.value.c_str()) : -atof(x.value.c_str()));
            case Digit:
                return VM::Variant(value == "true" ? 1 - atoi(x.value.c_str()) : -atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't sub from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(atof(value.c_str()) - (x.value == "true" ? 1 : 0));
            case Real:
                return VM::Variant(atof(value.c_str()) - atof(value.c_str()));
            case Digit:
                return VM::Variant(atof(value.c_str()) - atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't sub from value typed real to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(atoi(value.c_str()) - (x.value == "true" ? 1 : 0));
            case Real:
                return VM::Variant(atoi(value.c_str()) - atof(x.value.c_str()));
            case Digit:
                return VM::Variant(atoi(value.c_str()) - atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't sub from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            throw error<const char*>("Can't sub from value typed string to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::operator*(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(value == "true" && x.value == "true" ? 1 : 0);
            case Real:
                return VM::Variant(value == "true" ? atof(x.value.c_str()) : 0);
            case Digit:
                return VM::Variant(value == "true" ? atoi(x.value.c_str()) : 0);
            case String:
                throw error<const char*>("Can't mul from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(x.value == "true" ? atof(value.c_str()) : 0);
            case Real:
                return VM::Variant(atof(value.c_str()) * atof(value.c_str()));
            case Digit:
                return VM::Variant(atof(value.c_str()) * atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't mul from value typed real to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                return VM::Variant(x.value == "true" ? atoi(value.c_str()) : 0);
            case Real:
                return VM::Variant(atoi(value.c_str()) * atof(x.value.c_str()));
            case Digit:
                return VM::Variant(atoi(value.c_str()) * atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't mul from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            throw error<const char*>("Can't mul from value typed string to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::operator/(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                if (x.value == "false")
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(value == "true" ? 1 : 0);
            case Real:
                if (atof(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(value == "true" ? 1 / atof(x.value.c_str()) : 0);
            case Digit:
                if (atoi(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(value == "true" ? 1 / atoi(x.value.c_str()) : 0);
            case String:
                throw error<const char*>("Can't div from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
                if (x.value == "false")
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atof(value.c_str()));
            case Real:
                if (atof(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atof(value.c_str()) / atof(value.c_str()));
            case Digit:
                if (atoi(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atof(value.c_str()) / atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't div from value typed real to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                if (x.value == "false")
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atoi(value.c_str()));
            case Real:
                if (atof(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atoi(value.c_str()) / atof(x.value.c_str()));
            case Digit:
                if (atoi(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't divide into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atoi(value.c_str()) / atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't div from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            throw error<const char*>("Can't div from value typed string to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::operator%(const SyntaxTree_Value& x)const
    {
        switch (_type)
        {
        case Boolean:
            switch (x._type)
            {
            case Boolean:
                if (x.value == "false")
                {
                    throw error<const char*>("Can't modulo into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(value == "true" ? 1 : 0);
            case Real:
                throw error<const char*>("Can't mod from value typed bool to real", __FILE__, __LINE__);
                return VM::Variant();
            case Digit:
                if (atoi(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't modulo into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(value == "true" ? 1 % atoi(x.value.c_str()) : 0);
            case String:
                throw error<const char*>("Can't mod from value typed bool to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Real:
            switch (x._type)
            {
            case Boolean:
            case Real:
            case Digit:
                throw error<const char*>("Can't mod from value typed real to number", __FILE__, __LINE__);
                return VM::Variant();
            case String:
                throw error<const char*>("Can't mod from value typed real to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case Digit:
            switch (x._type)
            {
            case Boolean:
                if (x.value == "false")
                {
                    throw error<const char*>("Can't modulo into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atoi(value.c_str()) % 1);
            case Real:
                if (atof(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't modulo into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atoi(value.c_str()) / atof(x.value.c_str()));
            case Digit:
                if (atoi(x.value.c_str()) == 0)
                {
                    throw error<const char*>("Can't modulo into 0", __FILE__, __LINE__);
                    return VM::Variant();
                }
                return VM::Variant(atoi(value.c_str()) / atoi(x.value.c_str()));
            case String:
                throw error<const char*>("Can't mod from value typed digit to string", __FILE__, __LINE__);
                return VM::Variant();
            case Member: // TODO
                return VM::Variant();
            default:
                return VM::Variant();
            }
            break;
        case String:
            throw error<const char*>("Can't mod from value typed string to other type", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    const bool SyntaxTree_Value::toBool()const
    {
        switch (_type)
        {
        case Boolean:
            return value == "true" ? true : false;
        case Real:
            return atof(value.c_str()) ? true : false;
        case Digit:
            return atoi(value.c_str()) ? true : false;
        case String:
            throw error<const char*>("Can't convert from value typed string to bool", __FILE__, __LINE__);
            return false;
        case Member: // TODO
            return true;
        default:
            return false;
        }
    }

    VM::Variant SyntaxTree_Value::toVariant()const
    {
        switch (_type)
        {
        case Boolean:
            return VM::Variant(value == "true" ? true : false);
        case Real:
            return VM::Variant(atof(value.c_str()));
        case Digit:
            return VM::Variant(atoi(value.c_str()));
        case String:
            return VM::Variant(value);
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::toPositive()const
    {
        switch (_type)
        {
        case Boolean:
            return VM::Variant(value == "true" ? 1 : 0);
        case Real:
            return VM::Variant(+atof(value.c_str()));
        case Digit:
            return VM::Variant(+atoi(value.c_str()));
        case String:
            throw error<const char*>("string type have no opertor+", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
        }
    }

    VM::Variant SyntaxTree_Value::toNegative()const
    {
        switch (_type)
        {
        case Boolean:
            return VM::Variant(value == "true" ? -1 : 0);
        case Real:
            return VM::Variant(-atof(value.c_str()));
        case Digit:
            return VM::Variant(-atoi(value.c_str()));
        case String:
            throw error<const char*>("string type have no operator-", __FILE__, __LINE__);
            return VM::Variant();
        case Member: // TODO
            return VM::Variant();
        default:
            return VM::Variant();
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
