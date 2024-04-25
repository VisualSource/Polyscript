#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include "../../ast/Node.hpp"

namespace compiler
{
    class Compiler;

    class Target
    {
    private:
        std::string name;

    public:
        Target(std::string name) : name(name){};
        ~Target() {}

        inline std::string getName() { return this->name; }
        virtual void visitEntryPoint(std::vector<ast::Node *> &statements, Compiler *ctx) { throw std::logic_error("Invalid target"); }
    };
} // namespace compiler
