#pragma once
#include <vector>
#include <string>

#include "./Node.hpp"

namespace ast
{
    class Program
    {
    private:
        std::string name;
        std::vector<Node *> statements;

    public:
        Program();
        Program(std::string name, std::vector<Node *> statements) : name(name), statements(statements) {}
        ~Program();
        void setStatements(std::vector<Node *> statements);
        std::vector<Node *> &getStatements() const;
        std::string toString();
        std::string getName() { return name; };
    };
}