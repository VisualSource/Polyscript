#pragma once
#include <vector>

#include "./VariableDeclaration.hpp"
#include "./Consts.hpp"
#include "./Node.hpp"

namespace ast
{
    class VariableStatement : public Node
    {
    private:
        std::vector<VariableDeclaration *> declarations;

    public:
        VariableStatement(std::vector<VariableDeclaration *> declarations) : Node(0, 0, consts::VARIABLE_STATEMENT), declarations(declarations) {}
        ~VariableStatement()
        {
            for (auto &&i : declarations)
                if (i != nullptr)
                    delete i;
        }
        std::vector<VariableDeclaration *> &getDeclarations() { return declarations; };
        std::string toString(int padding = 0) override;
    };
} // namespace ast
