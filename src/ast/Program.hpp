#pragma once
#include <vector>
#include <iostream>

#include "./Node.hpp"

namespace ast
{
    class Program
    {
    private:
        std::vector<Node *> statements;

    public:
        Program();
        Program(std::vector<Node *> statements) : statements(statements) {}
        ~Program();
        void setStatements(std::vector<Node *> statements);
        std::vector<Node *> &getStatements() const;
    };
}