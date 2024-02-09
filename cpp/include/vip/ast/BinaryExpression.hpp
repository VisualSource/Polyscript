#pragma once
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class BinaryExpression : public Node
    {
    private:
        unsigned int op;
        Node *lhs;
        Node *rhs;

    public:
        BinaryExpression(unsigned int op, Node *lhs, Node *rhs) : Node(0, 0, consts::BINARY_EXPRESSION), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
        ~BinaryExpression();
        inline Node *getLhs() { return lhs; }
        inline Node *getRhs() { return rhs; }
        inline unsigned int getOp() const { return op; }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
