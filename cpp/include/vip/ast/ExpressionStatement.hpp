#pragma once
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{

    class ExpressionStatement : public Node
    {
    private:
        Node *expression;

    public:
        ExpressionStatement(Node *expression) : Node(0, 0, consts::EXPRESSION_STATEMENT), expression(std::move(expression)) {}
        ~ExpressionStatement();
        Node *getExpression() { return expression; }
        std::string toString(int padding = 0) override;
    };

}