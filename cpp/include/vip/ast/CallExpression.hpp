#pragma once
#include <vector>
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class CallExpression : public Node
    {
    private:
        Node *expression;
        std::vector<Node *> arguments;

    public:
        CallExpression(Node *expression, std::vector<Node *> arguments) : Node(0, 0, consts::CALL_EXPRESSION), expression(expression), arguments(arguments) {}
        ~CallExpression();
        inline Node *getExpression() { return expression; }
        inline std::vector<Node *> &getArguments() { return arguments; }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
