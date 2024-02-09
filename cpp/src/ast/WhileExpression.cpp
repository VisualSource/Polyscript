#include <vip/ast/WhileExpression.hpp>

namespace ast
{
    WhileExpression::~WhileExpression()
    {
        delete body;
        delete expression;
    }
    std::string WhileExpression::toString(int padding)
    {
        auto tag = std::string("<WhileExpression>\n").insert(0, padding, ' ');

        tag += std::string("<ExpressionStatement>\n").insert(0, padding + 3, ' ');
        tag += expression->toString(padding + 6);
        tag += std::string("</ExpressionStatement>\n").insert(0, padding + 3, ' ');
        tag += body->toString(padding + 3);
        tag += std::string("</WhileExpression>\n").insert(0, padding, ' ');

        return tag;
    }
} // namespace ast
