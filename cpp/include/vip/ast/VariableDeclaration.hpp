#pragma once
#include "./Identifier.hpp"
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class VariableDeclaration : public Node
    {
    private:
        Identifier *name;
        Identifier *type;
        Node *initializer;

    public:
        VariableDeclaration(Identifier *name, Identifier *type, Node *initializer) : Node(0, 0, consts::VARIABLE_DECLARATION), name(name), type(type), initializer(initializer) {}
        ~VariableDeclaration();
        Identifier *getName() { return name; }
        Identifier *getType() { return type; }
        Node *getInitalizer() { return initializer; }
        std::string toString(int padding = 0) override;
    };
}