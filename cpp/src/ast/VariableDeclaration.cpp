#include <vip/ast/VariableDeclaration.hpp>

namespace ast
{
    VariableDeclaration::~VariableDeclaration()
    {
        delete name;
        if (type != nullptr)
            delete type;
        if (initializer != nullptr)
            delete initializer;
    }

    std::string VariableDeclaration::toString(int padding)
    {
        auto tag = std::string("<VariableDeclaration>\n").insert(0, padding, ' ');
        tag += std::string("<Name>\n").insert(0, padding + 3, ' ');
        tag += name->toString(padding + 6);
        tag += std::string("</Name>\n").insert(0, padding + 3, ' ');

        tag += std::string("<Type>\n").insert(0, padding + 3, ' ');
        tag += type->toString(padding + 6);
        tag += std::string("</Type>\n").insert(0, padding + 3, ' ');

        if (initializer != nullptr)
        {
            tag += std::string("<Initializer>\n").insert(0, padding + 3, ' ');
            tag += initializer->toString(padding + 6);
            tag += std::string("</Initializer>\n").insert(0, padding + 3, ' ');
        }

        tag += std::string("</VariableDeclaration>\n").insert(0, padding, ' ');

        return tag;
    }
} // namespace ast
