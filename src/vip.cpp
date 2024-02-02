#include <vip/vip.hpp>
#include <string.h>
#include <iostream>
#include <ctype.h>
#include <deque>
#include "./tokenizer/Token.hpp"
#include "./ast/Parser.hpp"

static char ALLOWED_SYMBOLS[] = "{}()!;+=,-*&|#";

bool next(std::string &input, unsigned int &index, char &current)
{
    index++;
    if (index > input.length())
        return false;

    current = input[index];

    return true;
}

void vip::vipJit(std::string input)
{
    std::deque<Token> *tokens = new std::deque<Token>();
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

            tokens->push_back(Token(value, TYPE_IDENTIFER));
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

            tokens->push_back(Token(value, TYPE_STRING));

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

            tokens->push_back(Token(value, TYPE_NUMBER));
            continue;
        }

        if (strchr(ALLOWED_SYMBOLS, current) != nullptr)
        {
            tokens->push_back(Token(current, TYPE_SYMBOLE));
        }

        next(input, index, current);
    }

    ast::Parser parser = ast::Parser(tokens);

    ast::Program program = parser.parse();

    for (auto &&i : program.getStatements())
    {
        std::cout << i->toString() << std::endl;
    }

    delete tokens;
}