#pragma once
#include "./Node.hpp"

namespace ast
{
    class Identifier : public Node
    {
    private:
        std::string value;

    public:
        Identifier(std::string value) : value(value) {}
    };
}