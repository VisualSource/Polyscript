#pragma once
#include "./Consts.hpp"
#include "./Block.hpp"
#include "./Node.hpp"

namespace ast
{
    class IfStatement : public Node
    {
    private:
        Node *expression;
        Block *thenStatement;
        Node *elseStatement;

    public:
        IfStatement(Node *expression, Block *thenStatement, Node *elseStatement) : Node(0, 0, consts::IF_STATEMENT), expression(expression), thenStatement(thenStatement), elseStatement(elseStatement) {}
        ~IfStatement();
        std::string toString(int padding = 0) override;
    };
} // namespace ast
