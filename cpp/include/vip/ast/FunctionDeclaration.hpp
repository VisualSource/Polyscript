#pragma once

#include "./Identifier.hpp"
#include "./Parameter.hpp"
#include "./Consts.hpp"
#include "./Block.hpp"
#include "./Node.hpp"

namespace ast
{
    class FunctionDeclartion : public Node
    {
    private:
        Identifier *name;
        std::vector<Parameter *> parameters;
        Block *body;

    public:
        FunctionDeclartion(Identifier *name, std::vector<Parameter *> parameters, Block *body) : Node(0, 0, consts::FUNCTION_EXPRESSION), name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)) {}
        ~FunctionDeclartion();
        inline std::vector<Parameter *> getParameters() { return parameters; }
        inline void setBody(Block *body) { this->body = body; }
        inline void clearParams()
        {
            parameters.clear();
        }
        inline std::string &getName() { return name->getValue(); }
        inline Block *getBodyBlock() { return body; }
        inline std::vector<Node *> &getBody() { return body->getStatements(); }
        std::string toString(int padding = 0) override;
    };
} // namespace ast
