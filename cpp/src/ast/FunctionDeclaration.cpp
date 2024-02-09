#include <vip/ast/FunctionDeclaration.hpp>

namespace ast
{
    FunctionDeclartion::~FunctionDeclartion()
    {
        if (name != nullptr)
            delete name;
        if (body != nullptr)
            delete body;

        for (auto &&arg : parameters)
        {
            if (arg != nullptr)
                delete arg;
        }
    }
    // <FunctionDeclartion>
    //     <Parameters>
    //        <Parameter type="string" name="arg"/>
    //     </Parameters>
    //     <Name>
    //        <Identifier value="functionName"/>
    //     </Name>
    //     <Body>
    //        <Block>
    //            <ReturnStatement>
    //                 <StringLiteral value="Some string value"/>
    //            </ReturnStatement>
    //         </Block>
    //     </Body>
    // </FunctionDeclartion>
    std::string FunctionDeclartion::toString(int padding)
    {
        // start tag
        auto tag = std::string("<FunctionDeclartion>\n").insert(0, padding, ' ');

        // start parameters tag
        tag += std::string("<Parameters>\n").insert(0, padding + 4, ' ');
        for (auto &&i : parameters)
        {
            tag += i->toString(padding + 7);
        }
        tag += std::string("</Parameters>\n").insert(0, padding + 4, ' ');
        // end parameters tag

        // start name tag
        tag += std::string("<Name>\n").insert(0, padding + 3, ' ');
        tag += name->toString(padding + 7);
        tag += std::string("</Name>\n").insert(0, padding + 3, ' ');
        // end name tag

        // start body tag
        tag += std::string("<Body>\n").insert(0, padding + 3, ' ');
        tag += body->toString(padding + 7);
        tag += std::string("</Body>\n").insert(0, padding + 3, ' ');
        // end body tag

        // end tag
        tag += std::string("</FunctionDeclartion>\n").insert(0, padding, ' ');

        return tag;
    }
} // namespace ast
