#pragma once
#include <string>

namespace tokenizer
{
    const unsigned int TYPE_IDENTIFER = 0;
    const unsigned int TYPE_STRING = 1;
    const unsigned int TYPE_NUMBER = 2;
    const unsigned int TYPE_SYMBOL = 3;
    const unsigned int TYPE_UNKNOWN = 4;

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
        unsigned int getType() const { return type; }
        std::string &getValue() { return value; }
    };
}
