#pragma once
#include <memory>
#include <utility>

#include "../ast/FunctionDeclaration.hpp"
#include "../ast/ExpressionStatement.hpp"
#include "../ast/VariableStatement.hpp"
#include "../ast/IfStatement.hpp"
#include "../ast/Program.hpp"
#include "./Context.hpp"
#include "./Object.hpp"

namespace jit
{
    class Runtime
    {
    private:
        Context *ctx;
        void visitVariableStatement(ast::VariableStatement *value, Context *context);
        void visitIfStatement(ast::IfStatement *value, Context *context);
        void visitFunctionDeclartion(ast::FunctionDeclartion *value, Context *context);
        void visitVariableDeclaration(ast::VariableDeclaration *value, Context *context);
        std::shared_ptr<Object> visitStatements(std::vector<ast::Node *> &statements, Context *context, bool returnable, bool returnLast = false);
        std::pair<std::shared_ptr<Object>, bool> visitStatement(ast::Node *statement, Context *context);
        std::shared_ptr<Object> visitExpression(ast::Node *value, Context *context);

    public:
        Runtime();
        Runtime(const Runtime &) = delete;
        Runtime &operator=(const Runtime &) = delete;
        ~Runtime();
        void declare(std::string key, std::shared_ptr<Object> value);
        void drop(std::string key);
        std::shared_ptr<Object> execute(ast::Program &program, bool returnLast = false);
    };
}
