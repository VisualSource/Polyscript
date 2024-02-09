#pragma once
#include <vector>
#include "../Object.hpp"
#include "../../ast/Parameter.hpp"
#include "../../ast/Block.hpp"

namespace jit
{
    class Function : public Object
    {
    private:
        std::string name;
        ast::Block *body;
        std::vector<ast::Parameter *> params;

    public:
        Function(std::string name, ast::Block *body, std::vector<ast::Parameter *> params) : Object(1), name(name), body(body), params(params) {}

        inline ast::Block *getBody() { return body; }
        inline std::vector<ast::Parameter *> &getParams() { return params; }

        void print(std::ostream &where) const override;
    };
}
