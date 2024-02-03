#pragma once
#include <vector>

#include "./Node.hpp"
#include "./VariableDeclaration.hpp"

namespace ast
{
    class VariableStatement : public Node
    {
    private:
        std::vector<VariableDeclaration *> declarations;

    public:
        VariableStatement(std::vector<VariableDeclaration *> declarations) : declarations(declarations) {}
        ~VariableStatement()
        {
            for (auto &&i : declarations)
                if (i != nullptr)
                    delete i;
        }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
