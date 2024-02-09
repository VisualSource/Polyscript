#pragma once
#include "./Consts.hpp"
#include "./Node.hpp"
#include "Block.hpp"

namespace ast
{
    class WhileExpression : public Node
    {
    private:
        Node *expression;
        Block *body;

    public:
        WhileExpression(Node *expression, Block *body) : Node(0, 0, consts::WHILE_EXRESSION), expression(expression), body(body) {}
        ~WhileExpression();
        inline Node *getExpression() { return expression; }
        inline Block *getBody() { return body; }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
