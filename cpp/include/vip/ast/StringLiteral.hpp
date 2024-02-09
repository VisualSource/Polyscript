#pragma once
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class StringLiteral : public Node
    {
    private:
        std::string value;

    public:
        StringLiteral(std::string value) : Node(0, 0, consts::STRING_LITERAL), value(value) {}
        std::string toString(int padding = 0) override
        {
            return std::string("<StringLiteral value=\"" + value + "\"/>\n").insert(0, padding, ' ');
        }
        std::string &getValue() { return value; }
    };
} // namespace ast
