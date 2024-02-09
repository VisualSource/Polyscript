#pragma once
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{

    class NumericLiteral : public Node
    {
    private:
        double value;

    public:
        NumericLiteral(double value) : Node(0, 0, consts::NUMBERIC_LITERAL), value(value) {}
        double getValue() { return value; }
        std::string toString(int padding = 0) override;
    };
}