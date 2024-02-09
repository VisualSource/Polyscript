#pragma once
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class Identifier : public Node
    {
    private:
        std::string value;

    public:
        Identifier(std::string value) : Node(0, 0, consts::IDENTIFIER), value(value) {}
        std::string toString(int padding = 0) override;
        std::string &getValue() { return value; }
    };
}