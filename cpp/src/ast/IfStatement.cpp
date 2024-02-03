#include "./IfStatement.hpp"

namespace ast
{
    IfStatement::~IfStatement()
    {
        if (expression != nullptr)
            delete expression;
        if (thenStatement != nullptr)
            delete thenStatement;
        if (elseStatement != nullptr)
            delete elseStatement;
    }

    std::string IfStatement::toString(int padding)
    {
        auto tag = std::string("<IfStatement>\n").insert(0, padding, ' ');
        tag += std::string("<Expression>\n").insert(0, padding + 3, ' ');
        tag += expression->toString(padding + 6);
        tag += std::string("</Expression>\n").insert(0, padding + 3, ' ');
        tag += std::string("<ThenBlock>\n").insert(0, padding + 3, ' ');
        tag += thenStatement->toString(padding + 6);
        tag += std::string("</ThenBlock>\n").insert(0, padding + 3, ' ');

        if (elseStatement != nullptr)
        {
            tag += std::string("<ElseStatement>\n").insert(0, padding + 3, ' ');
            tag += elseStatement->toString(padding + 6);
            tag += std::string("</ElseStatement>\n").insert(0, padding + 3, ' ');
        }

        tag += std::string("</IfStatement>\n").insert(0, padding, ' ');

        return tag;
    }
} // namespace ast
