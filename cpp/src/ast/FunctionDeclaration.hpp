#pragma once
#include "./Identifier.hpp"
#include "./Parameter.hpp"
#include "./Block.hpp"
#include "./Node.hpp"

namespace ast
{
    class FunctionDeclartion : public Node
    {
    private:
        Identifier *name;
        std::vector<Parameter *> parameters;
        Block *body;

    public:
        FunctionDeclartion(Identifier *name, std::vector<Parameter *> parameters, Block *body) : name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)) {}
        ~FunctionDeclartion();
        std::string toString(int padding = 0) override;
    };
} // namespace ast
