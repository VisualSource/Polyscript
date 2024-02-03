#include "./VariableStatement.hpp"

namespace ast
{
    std::string VariableStatement::toString(int padding)
    {
        auto tag = std::string("<VariableStatement>\n").insert(0, padding, ' ');

        for (auto &&i : declarations)
        {
            tag += i->toString(padding + 3);
        }

        tag += std::string("</VariableStatement>\n").insert(0, padding, ' ');

        return tag;
    }
}