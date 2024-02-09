#pragma once

#include <vector>

#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class Block : public Node
    {
    private:
        std::vector<Node *> statements;

    public:
        Block(std::vector<Node *> statements) : Node(0, 0, consts::BLOCK_EXPRESSION), statements(std::move(statements)) {}
        ~Block();
        std::string toString(int padding = 0) override;
        std::vector<Node *> &getStatements() { return statements; }
    };
} // namespace ast
