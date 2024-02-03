#pragma once
#include <vector>
#include "./Node.hpp"

namespace ast
{
    class CallExpression : public Node
    {
    private:
        Node *expression;
        std::vector<Node *> arguments;

    public:
        CallExpression(Node *expression, std::vector<Node *> arguments) : expression(expression), arguments(arguments) {}
        ~CallExpression();
        Node *getExpression() { return expression; }
        std::vector<Node *> &getArguments() { return arguments; }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
