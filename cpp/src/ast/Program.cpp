#include "./Program.hpp"

namespace ast
{
    Program::Program()
    {
        statements = std::vector<Node *>();
    }
    Program::~Program()
    {
        for (auto &&i : statements)
            if (i != nullptr)
                delete i;
    }
    void Program::setStatements(std::vector<Node *> statements)
    {
        this->statements = statements;
    }

    std::vector<Node *> &Program::getStatements() const
    {
        return const_cast<std::vector<Node *> &>(statements);
    }
}
