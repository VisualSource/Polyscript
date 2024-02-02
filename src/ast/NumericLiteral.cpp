#include "./NumericLiteral.hpp"

namespace ast
{
    std::string NumericLiteral::toString(int padding)
    {
        return std::string("<NumericLiteral value=\"" + std::to_string(value) + "\"/>\n").insert(0, padding, ' ');
    }

}