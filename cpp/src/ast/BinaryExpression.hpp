#pragma once
#include "./Node.hpp"

namespace ast
{
    class BinaryExpression : public Node
    {
    private:
        char op;
        Node *lhs;
        Node *rhs;

    public:
        BinaryExpression(char op, Node *lhs, Node *rhs) : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
        ~BinaryExpression();
        std::string toString(int padding = 0) override;
    };
} // namespace ast
