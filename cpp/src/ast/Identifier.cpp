#include <vip/ast/Identifier.hpp>

namespace ast
{
    std::string Identifier::toString(int padding)
    {
        return std::string("<Identifier value=\"" + value + "\"/>\n").insert(0, padding, ' ');
    }
} // namespace ast
