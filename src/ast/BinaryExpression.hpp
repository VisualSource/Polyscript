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
        ~BinaryExpression()
        {
            if (lhs != nullptr)
                delete lhs;
            if (rhs != nullptr)
                delete rhs;
        }
        std::string toString(int padding = 0) override
        {
            return "<BinaryExpression\n" + lhs->toString(padding + 4) + "\n" + rhs->toString(padding + 4) + ">";
        }
    };
} // namespace ast
