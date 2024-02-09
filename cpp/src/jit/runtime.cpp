#include <vip/jit/runtime.hpp>
#include <stdexcept>
#include <vip/jit/components/InternalFunction.hpp>
#include <vip/jit/components/Function.hpp>
#include <vip/jit/components/String.hpp>
#include <vip/jit/components/Number.hpp>
#include <vip/ast/ReturnStatement.hpp>
#include <vip/ast/BinaryExpression.hpp>
#include <vip/ast/CallExpression.hpp>
#include <vip/ast/NumericLiteral.hpp>
#include <vip/ast/StringLiteral.hpp>
#include <vip/ast/Consts.hpp>

namespace jit
{
    Runtime::Runtime()
    {
        ctx = new Context("<root>", nullptr);
        ctx->set("false", std::shared_ptr<Number>(new Number(false)));
        ctx->set("true", std::shared_ptr<Number>(new Number(true)));
    }
    Runtime::~Runtime()
    {
        delete ctx;
    }

    void Runtime::declare(std::string key, std::shared_ptr<Object> value)
    {
        ctx->set(key, std::move(value));
    }
    void Runtime::drop(std::string key)
    {
        ctx->remove(key);
    }
    std::shared_ptr<Object> Runtime::execute(ast::Program &program, bool returnLast)
    {
        auto statements = program.getStatements();

        return visitStatements(statements, ctx, false, returnLast);
    }

    std::pair<std::shared_ptr<Object>, bool> Runtime::visitStatement(ast::Node *statement, Context *context)
    {
        switch (statement->getKind())
        {
        case ast::consts::VARIABLE_STATEMENT:
        { // Variale Statement
            if (auto *v = dynamic_cast<ast::VariableStatement *>(statement); v != nullptr)
            {
                visitVariableStatement(v, context);
                break;
            }
            throw std::runtime_error("Expected a variable statement.");
        }
        case ast::consts::IF_STATEMENT:
        { // if statement
            if (auto *v = dynamic_cast<ast::IfStatement *>(statement); v != nullptr)
            {
                visitIfStatement(v, context);
                break;
            }
            throw std::runtime_error("Expected a if statement.");
        }
        case ast::consts::FUNCTION_EXPRESSION:
        { // Function declartion
            if (auto *v = dynamic_cast<ast::FunctionDeclartion *>(statement); v != nullptr)
            {
                visitFunctionDeclartion(v, context);
                break;
            }
            throw std::runtime_error("Expected a function declartion statement.");
        }
        case ast::consts::EXPRESSION_STATEMENT:
        { // expression
            if (auto *v = dynamic_cast<ast::ExpressionStatement *>(statement); v != nullptr)
            {
                return std::make_pair(visitExpression(v->getExpression(), context), false);
            }
            throw std::runtime_error("Expected a expression statement statement.");
        }
        case ast::consts::RETURN_STATEMENT:
        { // return statement
            if (auto *v = dynamic_cast<ast::ReturnStatement *>(statement); v != nullptr)
            {
                return std::make_pair(visitExpression(v->getExpression(), context), true);
            }
            throw std::runtime_error("Uncaught SyntaxError: Illegal statement");
        }
        default:
            throw std::runtime_error("Uncaught SyntaxError: Illegal statement");
        }

        return std::make_pair(nullptr, false);
    }
    std::shared_ptr<Object> Runtime::visitStatements(std::vector<ast::Node *> &statements, Context *context, bool returnable, bool returnLast)
    {
        int last = statements.size() - 1;
        int idx = 0;
        for (auto &&i : statements)
        {

            auto [ptr, wasReturned] = visitStatement(i, context);
            if ((returnable && wasReturned) || (returnLast && (idx == last)))
                return ptr;
            else
            {
                throw std::runtime_error("Uncaught SyntaxError: Illegal return statement");
            }
            idx++;
        }

        return nullptr;
    }
    std::shared_ptr<Object> Runtime::visitExpression(ast::Node *value, Context *context)
    {
        if (auto bin = dynamic_cast<ast::BinaryExpression *>(value); bin != nullptr)
        {
            std::shared_ptr<Object> lhs = visitExpression(bin->getLhs(), context);
            std::shared_ptr<Object> rhs = visitExpression(bin->getRhs(), context);

            if (lhs == nullptr || rhs == nullptr)
            {
                throw std::runtime_error("Unable to operate on given types.");
            }

            if (lhs->getKind() != rhs->getKind())
            {
                throw std::runtime_error("Can not operate on two different types.");
            }

            switch (bin->getOp())
            {
            case ast::consts::AND:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(ln->asBool() && rn->asBool()));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::OR:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(ln->asBool() || rn->asBool()));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::GREATER_THEN:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln > *rn));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::GREATER_THEN_OR_EQUAL:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln >= *rn));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::LESS_THEN_OR_EQUAL:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln <= *rn));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::LESS_THEN:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln < *rn));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::MINUS:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln - *rn));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::DIV:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln / *rn));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::MULT:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln * *rn));
                }
                throw std::runtime_error("Invalid operation.");
            }
            case ast::consts::PLUS:
            {
                if (auto ln = std::dynamic_pointer_cast<Number>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<Number>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a number");

                    return std::shared_ptr<Number>(new Number(*ln + *rn));
                }
                else if (auto ln = std::dynamic_pointer_cast<String>(lhs); ln != nullptr)
                {
                    auto rn = std::dynamic_pointer_cast<String>(rhs);
                    if (rn == nullptr)
                        throw std::runtime_error("rhs does is not a string");

                    return std::shared_ptr<String>(new String(*ln + *rn));
                }

                throw std::runtime_error("Invalid operation.");
            }
            default:
                throw std::runtime_error("Unknown operation");
            }
        }
        else if (auto call = dynamic_cast<ast::CallExpression *>(value); call != nullptr)
        {
            auto name = dynamic_cast<ast::Identifier *>(call->getExpression());

            if (!context->has(name->getValue()))
            {
                throw std::runtime_error("No function with given name exists.");
            }

            auto fn = context->get(name->getValue());

            if (auto fnc = std::dynamic_pointer_cast<Function>(fn); fnc != nullptr)
            {
                auto fn_ctx = new Context("<function " + name->getValue() + ">", context, true);

                if (call->getArguments().size() != fnc->getParams().size())
                {
                    throw std::runtime_error("Given params does not function sig.");
                }
                // set params
                auto result = visitStatements(fnc->getBody()->getStatements(), fn_ctx, true);

                return result;
            }
            else if (auto ifn = std::dynamic_pointer_cast<InternalFunction>(fn); ifn != nullptr)
            {
                std::vector<std::shared_ptr<Object>> args;

                for (auto &&i : call->getArguments())
                {
                    args.push_back(visitExpression(i, context));
                }

                return ifn->execute(args);
            }

            throw std::runtime_error("Failed to execute function");
        }
        else if (auto num = dynamic_cast<ast::NumericLiteral *>(value); num != nullptr)
        {
            return std::shared_ptr<Number>(new Number(num->getValue()));
        }
        else if (auto str = dynamic_cast<ast::StringLiteral *>(value); str != nullptr)
        {
            return std::shared_ptr<String>(new String(str->getValue()));
        }
        else if (auto idnt = dynamic_cast<ast::Identifier *>(value); idnt != nullptr)
        {
            if (context->has(idnt->getValue()))
            {
                return context->get(idnt->getValue());
            }

            throw std::runtime_error("No variable exsists");
        }
        else
        {
            throw std::runtime_error("Unknown expression.");
        }
    }

    void Runtime::visitVariableDeclaration(ast::VariableDeclaration *value, Context *context)
    {
        std::string name = value->getName()->getValue();
        std::string type = value->getType()->getValue();

        auto init = value->getInitalizer();

        if (init == nullptr)
        {
            if (type == "string")
            {
                context->set(name, std::shared_ptr<String>());
            }
            else if (type == "number")
            {
                context->set(name, std::shared_ptr<Number>());
            }
            else
            {
                throw std::runtime_error("Unsupported type");
            }
            return;
        }

        context->set(name, visitExpression(init, context));
    }

    void Runtime::visitVariableStatement(ast::VariableStatement *value, Context *context)
    {
        for (auto &&i : value->getDeclarations())
        {
            visitVariableDeclaration(i, context);
        }
    }

    void Runtime::visitIfStatement(ast::IfStatement *value, Context *context)
    {
        (void)value;
        (void)context;
    }

    void Runtime::visitFunctionDeclartion(ast::FunctionDeclartion *value, Context *context)
    {
        auto fn = std::shared_ptr<Function>(new Function(value->getName(), value->getBodyBlock(), value->getParameters()));

        if (context->has(value->getName()))
        {
            throw std::runtime_error("A variable already exists with this name.");
        }

        context->set(value->getName(), fn);
    }

} // namespace jit
