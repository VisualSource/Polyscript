#include "./Token.hpp"

std::string getTypeString(unsigned int type)
{
    switch (type)
    {
    case TYPE_IDENTIFER:
        return "Identifer";
    case TYPE_NUMBER:
        return "Number";
    case TYPE_STRING:
        return "String";
    case TYPE_SYMBOLE:
        return "Symbole";
    default:
        return "UNKNOWN";
    }
}

Token::Token(char v, unsigned int type) : type(type)
{
    value = std::string(1, v);
}

Token::Token(std::string value, unsigned int type) : type(type), value(value) {}

std::string Token::toString()
{
    return "<" + getTypeString(type) + ":" + value + ">";
}