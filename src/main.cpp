#include <iostream>
#include <string.h>
#include <string>
#include <ctype.h>
#include <list>
#include "./helpers/Token.hpp"
#include "./helpers/Node.hpp"

const char ALLOWED_SYMBOLS[] = "{}()!;+=,-*&|#";

bool isKeyword(std::string &value)
{
    return value == "let" || value == "while" || value == "fn" || value == "if" || value == "else";
}

bool next(std::string &input, unsigned int &index, char &current)
{
    index++;
    if (index > input.length())
        return false;

    current = input[index];

    return true;
}

Token &consume(std::list<Token> &list)
{
    auto front = list.front();
    list.pop_front();
    return front;
}

bool consume_if(std::list<Token> &list, unsigned int type, std::string value)
{

    Token item = list.front();
    bool result = item.getType() == type && item.getValue() == value;

    if (result)
    {
        list.pop_front();
    }

    return result;
}

void parse_scope(std::list<Token> &list)
{
    // consume {

    // consume }
}

int main(int, char **)
{
    std::string input = "fn main(){ print(\"Hello, World\"); print(100.00) }";
    std::list<Token> tokens = std::list<Token>();

    unsigned int index = 0;
    char current = input[0];

    while (current != '\0')
    {

        if (isalpha(current))
        {
            std::string value = "";
            while (isalpha(current) || isdigit(current) || current == '.')
            {
                value.push_back(current);
                next(input, index, current);
            }

            tokens.push_back(Token(value, TYPE_IDENTIFER));
            continue;
        }

        if (current == '"')
        {
            next(input, index, current);
            std::string value = "";

            while (current != '"')
            {
                value.push_back(current);
                next(input, index, current);
            }

            next(input, index, current);

            tokens.push_back(Token(value, TYPE_STRING));

            continue;
        }

        if (isdigit(current))
        {
            bool seen_dot = false;

            std::string value = "";
            while (isdigit(current) || (current == '.' && !seen_dot))
            {
                if (current == '.')
                {
                    seen_dot = true;
                }

                value.push_back(current);
                next(input, index, current);
            }

            tokens.push_back(Token(value, TYPE_NUMBER));
            continue;
        }

        if (strchr(ALLOWED_SYMBOLS, current) != nullptr)
        {
            tokens.push_back(Token(current, TYPE_SYMBOLE));
        }

        next(input, index, current);
    }

    // key words "let","while","fn"

    std::list<Node> ast = std::list<Node>();
}