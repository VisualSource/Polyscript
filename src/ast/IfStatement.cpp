#include "./IfStatement.hpp"

namespace ast
{
    std::string IfStatement::toString(int padding)
    {
        auto header = std::string("<IfStatement\n").insert(0, padding, ' ');
        auto exp = expression->toString(padding + 3);
        auto t = thenStatement->toString(padding + 3);
        auto footer = std::string(">\n").insert(0, padding, ' ');
        if (elseStatement == nullptr)
        {
            return header + exp + t + footer;
        }

        auto e = elseStatement->toString(padding + 3);

        return header + exp + t + e + footer;
    }
} // namespace ast
