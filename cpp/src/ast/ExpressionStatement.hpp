#pragma once
#include "./Node.hpp"

namespace ast
{

    class ExpressionStatement : public Node
    {
    private:
        Node *expression;

    public:
        ExpressionStatement(Node *expression) : expression(std::move(expression)) {}
        ~ExpressionStatement();
        Node *getExpression() { return expression; }
        std::string toString(int padding = 0) override;
    };

}