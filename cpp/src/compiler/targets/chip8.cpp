#include <vip/compiler/targets/chip8.hpp>
#include <vip/ast/ReturnStatement.hpp>
#include <vip/ast/WhileExpression.hpp>
#include <vip/ast/BinaryExpression.hpp>
#include <vip/ast/CallExpression.hpp>
#include <vip/ast/Consts.hpp>
#include <stdexcept>

namespace compiler
{
    void X16ChipUnknown::visitEntryPoint(std::vector<ast::Node *> &statements, Compiler *ctx)
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

        Scope main;

        visitStatements(statements, ctx, main);

        // create output
    }
    void X16ChipUnknown::visitStatements(std::vector<ast::Node *> &statements, Compiler *ctx, Scope &scope)
    {
        for (auto &&statement : statements)
            visitStatement(statement, ctx, scope);
    }
    void X16ChipUnknown::visitStatement(ast::Node *statement, Compiler *context, Scope &scope)
    {
        switch (statement->getKind())
        {
        case ast::consts::VARIABLE_STATEMENT:
        {
            /**
             * Binary Expression
             *  8XY1,8XY2,8XY3,8XY4,8XY5,8XY6,8XY7,8XYE,FX33
             * ===================
             *
             * Function Call
             *
             *  Load params into register
             *  2NNN CALL
             * ==================
             *
             * NumericLiteral | StringLiteral
             * Encode and store
             * ==============================
             * Identifier
             * load value from memory in to register
             */

            if (auto *v = dynamic_cast<ast::VariableStatement *>(statement); v != nullptr)
                return visitVariableStatement(v, context, scope);

            throw std::runtime_error("Expected a variable statement");
        }
        case ast::consts::IF_STATEMENT:
        {
            if (auto *v = dynamic_cast<ast::IfStatement *>(statement); v != nullptr)
            {
                // for consts check
                // var == NN 3XNN, VAR != NN 4XNN

                // var == var 5XY0

                /***
                 * 5XY0
                 * 1NNN if(var == var)
                 * Body {}
                 * 1NNN SKIP Remaing branches
                 *
                 * 5XY0
                 * 1NNN else if()
                 *  BODY
                 * 1NNN skip remaing branches
                 * ELSE BODY else {}
                 */
            }

            throw std::runtime_error("Expected a if statement.");
        }
        case ast::consts::FUNCTION_EXPRESSION:
        {
            /**
             * ==== Function START
             * BODY
             * ?(Set register with return value)
             * 00EE RET
             * ===== FUNCTION END
             */

            if (auto *v = dynamic_cast<ast::FunctionDeclartion *>(statement); v != nullptr)
                return visitFunctionDeclartion(v, context, scope);
            throw std::runtime_error("Expected a function declartion statement.");
        }
        case ast::consts::EXPRESSION_STATEMENT:
        {
            if (auto v = dynamic_cast<ast::ExpressionStatement *>(statement); v != nullptr)
                return visitExpression(v->getExpression(), context, scope);
            throw std::runtime_error("Expected a expression statement statement.");
        }
        case ast::consts::RETURN_STATEMENT:
        {
            /** Store value in register */
            if (auto *v = dynamic_cast<ast::ReturnStatement *>(statement); v != nullptr)
                return visitExpression(v->getExpression(), context, scope);

            throw std::runtime_error("Uncaught SyntaxError: Illegal statement");
        }
        case ast::consts::WHILE_EXRESSION:
        {
            /**
             * if const just use loop
             * ==== LOOP
             * BODY
             * 1NNN
             * ========
             *
             * ==== WHILE LOOP
             * 3XNN,4XNN,5XY0 check
             * 1NNN EXIT LOOP
             *  BODY
             * 1NNN JMP bck to check
             * ============
             *
             */
            auto *w = dynamic_cast<ast::WhileExpression *>(statement);
            if (w == nullptr)
                throw std::runtime_error("Uncaught SyntaxError: Illegal statement");

            // visitStatements(w->getBody()->getStatements(), context, "scope");

            break;
        }
        default:
            throw std::runtime_error("Uncaught SyntaxError: Illegal statement");
        }
    }
    void X16ChipUnknown::visitIfStatement(ast::IfStatement *value, Compiler *context, Scope &scope) {}
    void X16ChipUnknown::visitVariableStatement(ast::VariableStatement *value, Compiler *context, Scope &scope) {}
    void X16ChipUnknown::visitFunctionDeclartion(ast::FunctionDeclartion *value, Compiler *context, Scope &scope) {}
    void X16ChipUnknown::visitVariableDeclaration(ast::VariableDeclaration *value, Compiler *context, Scope &scope) {}
    std::optional<compiler::Chip8Object> X16ChipUnknown::visitExpression(ast::Node *value, Compiler *context, Scope &scope)
    {
        switch (value->getKind())
        {
        case ast::consts::BINARY_EXPRESSION:
        {
            auto bin = dynamic_cast<ast::BinaryExpression *>(value);
            if (bin == nullptr)
                throw std::logic_error("Invalid Binary Expression");

            if (bin->getOp() == ast::consts::EQUAL)
            {
                throw std::logic_error("Not Implemented: EQUAL");
            }

            auto lhs = visitExpression(bin->getLhs(), context, scope);
            auto rhs = visitExpression(bin->getRhs(), context, scope);

            if (lhs == std::nullopt || rhs == std::nullopt)
                throw std::logic_error("Bad Binary Expression");

            if (lhs->getKind() != rhs->getKind())
                throw std::logic_error("Can not operate on two different types.");

            switch (bin->getOp())
            {
            case ast::consts::AND:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::OR:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::GREATER_THEN:
            {
            }
            case ast::consts::GREATER_THEN_OR_EQUAL:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::LESS_THEN_OR_EQUAL:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::LESS_THEN:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::MINUS:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::DIV:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::MULT:
            {
                throw std::logic_error("Not Implemented");
            }
            case ast::consts::PLUS:
            {
                throw std::logic_error("Not Implemented");
            }
            default:
                throw std::runtime_error("Unknown operation");
            }

            break;
        }
        case ast::consts::CALL_EXPRESSION:
        {
            auto call = dynamic_cast<ast::CallExpression *>(value);
            if (call == nullptr)
                throw std::logic_error("Invalid call expression");

            auto name = dynamic_cast<ast::Identifier *>(call->getExpression());
            if (name == nullptr)
                throw std::logic_error("Invalid Identifier");
        }
        case ast::consts::NUMBERIC_LITERAL:
            throw std::logic_error("Not Implemented: NUMBERIC_LITERAL");
        case ast::consts::STRING_LITERAL:
            throw std::logic_error("Not Implemented: STRING_LITERAL");
        case ast::consts::IDENTIFIER:
            throw std::logic_error("Not Implemented: IDENTIFIER");
        default:
            throw std::logic_error("Unknown expression");
        }
    }

}