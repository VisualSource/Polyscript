#pragma once

#include "./Identifier.hpp"
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class Parameter : public Node
    {
    private:
        Identifier *name;
        Node *type;
        Node *initializer;

    public:
        Parameter(Identifier *name, Node *type) : Node(0, 0, consts::PARAMETER_EXRESSION), name(std::move(name)), type(std::move(type)), initializer(nullptr) {}
        ~Parameter();
        inline Identifier *getName() const { return name; }
        inline Node *getType() const { return type; }
        inline Node *getInitializer() const { return initializer; }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
