#include <vip/compiler/targets/chip8.hpp>
#include <vip/ast/Consts.hpp>
#include <stdexcept>

namespace compiler
{
    void X16ChipUnknown::visitEntryPoint(std::vector<ast::Node *> &statements, Compiler *ctx) override
    {

        /***
         *  Functions Section
         *  1. imports
         *  2. entrypoint
         *  Instruction Sections
         *  1. imports
         *  2. entrypoint
         */

        // parse into byte code

        visitStatements(statements, ctx, "body");

        // create output
    }
    void X16ChipUnknown::visitStatements(std::vector<ast::Node *> &statements, Compiler *ctx, std::string scope) override
    {
        for (auto &&statement : statements)
        {
            visitStatement(i, context, scope);
        }
    }
    void X16ChipUnknown::visitStatement(ast::Node *statement, Compiler *context, std::string scope) override
    {
        switch (statement->getKind())
        {
        case ast::consts::VARIABLE_STATEMENT:
        {
            break;
        }
        case ast::consts::IF_STATEMENT:
        {
            break;
        }
        case ast::consts::FUNCTION_EXPRESSION:
        {
            break;
        }
        case ast::consts::EXPRESSION_STATEMENT:
        {
            break;
        }
        case ast::consts::RETURN_STATEMENT:
        {
            break;
        }
        case ast::consts::WHILE_EXPRESSION:
        {
            break;
        }
        }
    }
    void X16ChipUnknown::visitVariableStatement(ast::VariableStatement *value, Compiler *context, std::string scope) override {}
    void X16ChipUnknown::visitFunctionDeclartion(ast::FunctionDeclartion *value, Compiler *context, std::string scope) override {}
    void X16ChipUnknown::visitVariableDeclaration(ast::VariableDeclaration *value, Compiler *context, std::string scope) override {}
    void X16ChipUnknown::visitExpression(ast::Node *value, Compiler *context, std::string scope) override {}

}