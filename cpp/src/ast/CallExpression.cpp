#include <vip/ast/CallExpression.hpp>

namespace ast
{
    CallExpression::~CallExpression()
    {
        if (expression != nullptr)
            delete expression;

        if (arguments.empty())
            return;
        for (auto &&i : arguments)
        {
            if (i != nullptr)
                delete i;
        }
    }

    std::string CallExpression::toString(int padding)
    {
        auto tag = std::string("<CallExpression>\n").insert(0, padding, ' ');

        tag += std::string("<Expression>\n").insert(0, padding + 3, ' ');
        tag += expression->toString(padding + 6);
        tag += std::string("</Expression>\n").insert(0, padding + 3, ' ');

        tag += std::string("<Arguments>\n").insert(0, padding + 3, ' ');
        for (auto &&i : arguments)
        {
            tag += i->toString(padding + 6);
        }
        tag += std::string("</Arguments>\n").insert(0, padding + 3, ' ');

        tag += std::string("</CallExpresion>\n").insert(0, padding, ' ');
        return tag;
    }
} // namespace ast
