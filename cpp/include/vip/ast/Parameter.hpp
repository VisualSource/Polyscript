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
        std::string toString(int padding = 0) override;
    };
} // namespace ast
