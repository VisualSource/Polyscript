#pragma once
#include "./Node.hpp"
#include "./Identifier.hpp"
#include "./Block.hpp"
namespace ast
{
    class FunctionDeclartion : public Node
    {
    private:
        Identifier *name;
        Block *body;

    public:
        FunctionDeclartion(Identifier *name, Block *body) : name(std::move(name)), body(std::move(body)) {}
        ~FunctionDeclartion()
        {
            delete name;
            delete body;
        }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
