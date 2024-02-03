#include "./Block.hpp"

namespace ast
{
    Block::~Block()
    {
        for (auto &&i : statements)
            if (i != nullptr)
                delete i;
    }

    std::string Block::toString(int padding)
    {

        std::string header = std::string("<Block>\n").insert(0, padding, ' ');
        std::string body;
        for (auto &&i : statements)
        {
            body += i->toString(padding + 6);
        }
        std::string footer = std::string("</Block>\n").insert(0, padding, ' ');

        return header + body + footer;
    }
} // namespace ast
