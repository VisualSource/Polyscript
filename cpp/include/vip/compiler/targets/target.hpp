#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include "../../ast/Node.hpp"
#include "../../ast/FunctionDeclaration.hpp"
#include "../../ast/ExpressionStatement.hpp"
#include "../../ast/VariableStatement.hpp"
#include "../../ast/IfStatement.hpp"
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
        virtual void visitStatements(std::vector<ast::Node *> &statements, Compiler *ctx, std::string scope) { throw std::logic_error("Invalid target"); }
        virtual void visitStatement(ast::Node *statement, Compiler *context, std::string scope) { throw std::logic_error("Invalid target"); }
        virtual void visitVariableStatement(ast::VariableStatement *value, Compiler *context, std::string scope) { throw std::logic_error("Invalid target"); }
        virtual void visitFunctionDeclartion(ast::FunctionDeclartion *value, Compiler *context, std::string scope) { throw std::logic_error("Invalid target"); }
        virtual void visitVariableDeclaration(ast::VariableDeclaration *value, Compiler *context, std::string scope) { throw std::logic_error("Invalid target"); }
        virtual void visitExpression(ast::Node *value, Compiler *context, std::string scope) { throw std::logic_error("Invalid target"); }
    };
} // namespace compiler
