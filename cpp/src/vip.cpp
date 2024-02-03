#include <vip/vip.hpp>
#include <string.h>
#include <iostream>
#include <ctype.h>
#include <deque>
#include "./compile/compiler.hpp"
#include "./compile/astToCil.hpp"
#include "./tokenizer/Token.hpp"
#include "./ast/Parser.hpp"

namespace vip
{
    static char ALLOWED_SYMBOLS[] = "{}()!;:+=,-*&|#";

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

        delete tokens;

        return program;
    }

    void vipJit(std::string input)
    {
        ast::Program program = tokenize(input);

        std::cout << program.toString() << std::endl;
    }

    void vipCompile(std::string input)
    {
        ast::Program program = tokenize(input);

        std::vector<std::string> files = compile::astToCil(program, "../../", "vip_build/");
        compile::compileCil(files, std::vector<std::string>(), "../../Program.o");

        // std::cout << program.toString() << std::endl;
    }

} // namespace vip
