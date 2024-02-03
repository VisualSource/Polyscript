#pragma once
#include "./Node.hpp"
#include "./Identifier.hpp"

namespace ast
{
    class Parameter : public Node
    {
    private:
        Identifier *name;
        Node *type;
        Node *initializer;

    public:
        Parameter(Identifier *name, Node *type) : name(std::move(name)), type(std::move(type)), initializer(nullptr) {}
        ~Parameter()
        {
            delete name;
            if (type != nullptr)
                delete type;
        }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
