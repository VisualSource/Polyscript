#pragma once
#include "./Node.hpp"
#include "./Block.hpp"

namespace ast
{
    class IfStatement : public Node
    {
    private:
        Node *expression;
        Block *thenStatement;
        Node *elseStatement;

    public:
        IfStatement(Node *expression, Block *thenStatement, Node *elseStatement) : expression(expression), thenStatement(thenStatement), elseStatement(elseStatement) {}
        ~IfStatement();
        std::string toString(int padding = 0) override;
    };
} // namespace ast
