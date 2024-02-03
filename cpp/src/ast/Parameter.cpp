#include "./Parameter.hpp"

namespace ast
{
    Parameter::~Parameter()
    {
        delete name;
        if (initializer != nullptr)
            delete initializer;
        if (type != nullptr)
            delete type;
    }
    //  <Parameter>
    //    <Name>
    //      <Identifier value=""/>
    //    </Name>
    //    <Type>
    //      <Identifier value=""/>
    //    </Type>
    //  </Parameter>
    std::string Parameter::toString(int padding)
    {
        auto tag = std::string("<Parameter>\n").insert(0, padding, ' ');
        tag += std::string("<Name>\n").insert(0, padding + 3, ' ');
        tag += name->toString(padding + 6);
        tag += std::string("</Name>\n").insert(0, padding + 3, ' ');

        if (type != nullptr)
        {
            tag += std::string("<Type>\n").insert(0, padding + 3, ' ');
            tag += type->toString(padding + 6);
            tag += std::string("</Type>\n").insert(0, padding + 3, ' ');
        }

        tag += std::string("</Parameter>\n").insert(0, padding, ' ');

        return tag;
    }
} // namespace ast
