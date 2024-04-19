#include <vip/vip.hpp>

#include <string.h>
#include <iostream>
#include <ctype.h>
#include <deque>

#include <vip/tokenizer/Token.hpp>
#include <vip/ast/Parser.hpp>

namespace vip
{
    static char ALLOWED_SYMBOLS[] = "{}()!;:+=/,<>-*&|#";

    bool isDoubleOperator(char input, char next)
    {
        switch (input)
        {
        case '!':
        {
            if (next == '=')
                return true;
            return false;
        }
        case '&':
        {
            if (next == '&')
                return true;
            return false;
        }
        case '|':
        {
            if (next == '|')
                return true;
            return false;
        }
        case '+':
        {
            if (next == '=')
                return true;
            return false;
        }
        case '-':
        {
            if (next == '=')
                return true;
            return false;
        }
        case '=':
        {
            if (next == '=')
                return true;
            return false;
        }
        case '>':
        {
            if (next == '=')
                return true;
            return false;
        }
        case '<':
        {
            if (next == '>')
                return true;
            return false;
        }
        default:
            return false;
        }
    }

    bool next(std::string &input, unsigned int &index, char &current)
    {
        index++;
        if (index > input.length())
            return false;

        current = input[index];

        return true;
    }

    ast::Program tokenize(std::string input)
    {
        std::deque<tokenizer::Token> *tokens = new std::deque<tokenizer::Token>();
        unsigned int index = 0;
        char current = input[0];

        while (current != '\0')
        {
            if (iscntrl(current))
            {
                next(input, index, current);
                continue;
            }

            if (isalpha(current))
            {
                std::string value = "";
                while (isalpha(current) || isdigit(current) || current == '.')
                {
                    value.push_back(current);
                    next(input, index, current);
                }

                tokens->push_back(tokenizer::Token(value, tokenizer::TYPE_IDENTIFER));
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

                tokens->push_back(tokenizer::Token(value, tokenizer::TYPE_STRING));

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

                tokens->push_back(tokenizer::Token(value, tokenizer::TYPE_NUMBER));
                continue;
            }

            if (strchr(ALLOWED_SYMBOLS, current) != nullptr)
            {
                if (isDoubleOperator(current, input[index + 1]))
                {
                    std::string item;
                    item.push_back(current);
                    next(input, index, current);
                    item.push_back(current);
                    tokens->push_back(tokenizer::Token(item, tokenizer::TYPE_SYMBOL));
                }
                else
                {
                    tokens->push_back(tokenizer::Token(current, tokenizer::TYPE_SYMBOL));
                }
            }

            next(input, index, current);
        }

        ast::Parser parser = ast::Parser(tokens);

        ast::Program program = parser.parse();

        delete tokens;

        return program;
    }

} // namespace vip
