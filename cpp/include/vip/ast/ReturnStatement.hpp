#pragma once
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class ReturnStatement : public Node
    {
    private:
        Node *expression;

    public:
        ReturnStatement(Node *expression) : Node(0, 0, consts::RETURN_STATEMENT), expression(expression) {}
        ~ReturnStatement() { delete expression; }
        Node *getExpression() { return expression; }
        std::string toString(int padding = 0) override
        {
            auto tag = std::string("<ReturnStatement>\n").insert(0, padding, ' ');
            tag += expression->toString(padding + 3);
            tag += std::string("</ReturnStatement>\n").insert(0, padding, ' ');
            return tag;
        }
    };
} // namespace ast
