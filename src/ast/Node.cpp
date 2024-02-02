#include "./Node.hpp"

namespace ast
{
    std::string Node::toString(int padding)
    {
        return std::string("<Node>\n").insert(0, padding, ' ');
    }
}