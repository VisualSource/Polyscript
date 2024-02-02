#include "./FunctionDeclaration.hpp"

namespace ast
{
    std::string FunctionDeclartion::toString(int padding)
    {
        auto header = std::string("<FunctionDeclartion>\n");
        auto a = name->toString(padding + 7);
        auto b = body->toString(padding + 7);
        auto footer = std::string("</FunctionDeclartion>\n").insert(0, padding, ' ');

        return header + a + b + footer;
    }
} // namespace ast
