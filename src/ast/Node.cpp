#include "./Node.hpp"
#define UNUSED(x) (void)(x)
namespace ast
{
    std::string Node::toString(int padding)
    {
        std::string data = std::string("<Node>");
        UNUSED(padding);
        return data;
    }
}