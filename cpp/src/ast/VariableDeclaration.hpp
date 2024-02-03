#pragma once
#include "./Node.hpp"
#include "./Identifier.hpp"

namespace ast
{
    class VariableDeclaration : public Node
    {
    private:
        Identifier *name;
        Identifier *type;
        Node *initializer;

    public:
        VariableDeclaration(Identifier *name, Identifier *type, Node *initializer) : name(name), type(type), initializer(initializer) {}
        ~VariableDeclaration();
        std::string toString(int padding = 0) override;
    };
}