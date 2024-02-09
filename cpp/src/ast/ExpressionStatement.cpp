#include <vip/ast/ExpressionStatement.hpp>

namespace ast
{
    ExpressionStatement::~ExpressionStatement()
    {
        if (expression != nullptr)
            delete expression;
    }
    std::string ExpressionStatement::toString(int padding)
    {
        std::string header = std::string("<ExpressionStatement>\n").insert(0, padding, ' ');
        std::string item = expression->toString(padding + 6) + "\n";
        std::string footer = std::string("</ExpressionStatement>\n").insert(0, padding, ' ');
        return header + item + footer;
    }
} // namespace ast
