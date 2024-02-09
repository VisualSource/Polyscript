#pragma once
#include <vector>
#include "../../ast/Parameter.hpp"
#include "../../ast/Block.hpp"
#include "../Object.hpp"
#include "../Consts.hpp"

namespace jit
{
    class Function : public Object
    {
    private:
        std::string name;
        ast::Block *body;
        std::vector<ast::Parameter *> params;

    public:
        Function(std::string name, ast::Block *body, std::vector<ast::Parameter *> params) : Object(consts::ID_FUNCTION), name(name), body(body), params(params) {}
        ~Function();
        inline ast::Block *getBody() { return body; }
        inline std::vector<ast::Parameter *> &getParams() { return params; }

        void print(std::ostream &where) const override;
    };
}
