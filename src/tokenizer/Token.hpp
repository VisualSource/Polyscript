#pragma once
#include <string>

#define TYPE_IDENTIFER 0
#define TYPE_STRING 1
#define TYPE_NUMBER 2
#define TYPE_SYMBOLE 3
#define TYPE_UNKNOWN 4;

class Token
{
private:
    unsigned int type;
    std::string value;

public:
    Token()
    {
        value = "";
        type = TYPE_UNKNOWN;
    }
    Token(std::string value, unsigned int type);
    Token(char value, unsigned int type);
    std::string toString();
    unsigned int getType() { return type; }
    std::string &getValue() { return value; }
};