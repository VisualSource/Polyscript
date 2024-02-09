#include <vip/ast/Parser.hpp>
#include <stdexcept>
#include <string.h>
#include <string>

#include <vip/ast/ExpressionStatement.hpp>
#include <vip/ast/VariableDeclaration.hpp>
#include <vip/ast/BinaryExpression.hpp>
#include <vip/ast/ReturnStatement.hpp>
#include <vip/ast/CallExpression.hpp>
#include <vip/ast/NumericLiteral.hpp>
#include <vip/ast/StringLiteral.hpp>
#include <vip/ast/Identifier.hpp>
#include <vip/ast/Parameter.hpp>
#include <vip/ast/Consts.hpp>
#include <vip/ast/Block.hpp>

namespace ast
{
    unsigned int getOperatorValue(std::string &value)
    {
        if (value.size() == 1)
        {
            return value.at(0);
        }
        if (value.size() > 2)
            throw std::logic_error("operator should be at most 2 chars in length");
        return value.at(0) + value.at(1);
    }

    Parser::Parser(std::deque<tokenizer::Token> *tokens) : tokens(tokens), current(tokenizer::Token())
    {
        // setup first token
        consume();
    }
    void Parser::consume()
    {
        if (tokens->empty())
        {
            return;
        }
        current = tokenizer::Token(tokens->front());
        tokens->pop_front();
    }
    bool Parser::is(unsigned int type)
    {
        return current.getType() == type;
    }
    bool Parser::is(unsigned int type, char value)
    {
        return is(type) && current.getValue().at(0) == value;
    }
    bool Parser::is(unsigned int type, std::string value)
    {
        return is(type) && current.getValue() == value;
    }
    bool Parser::is_next(unsigned int type)
    {
        if (tokens->empty())
            return false;

        auto ref = tokens->front();

        return ref.getType() == type;
    }
    bool Parser::is_next(unsigned int type, char value)
    {
        if (tokens->empty())
            return false;

        auto ref = tokens->front();

        return ref.getType() == type && ref.getValue().at(0) == value;
    }
    bool Parser::is_next(const char *check)
    {
        if (tokens->empty())
            return false;
        auto ref = tokens->front();
        if (ref.getType() != tokenizer::TYPE_SYMBOL)
            return false;

        return strchr(check, ref.getValue().at(0)) != nullptr;
    }
    int Parser::is_keyword()
    {
        auto value = current.getValue();
        if (!(is(tokenizer::TYPE_IDENTIFER) && (value == "fn" || value == "let" || value == "if" || value == "return")))
            return -1;

        if (value == "fn")
            return 0;
        if (value == "let")
            return 1;
        if (value == "if")
            return 2;
        if (value == "return")
            return 3;

        return -1;
    }
    bool Parser::is_any(const char *check)
    {
        if (current.getType() != tokenizer::TYPE_SYMBOL)
            return false;
        return strchr(check, current.getValue().at(0)) != nullptr;
    }
    int Parser::getBinopPrecedence()
    {
        if (!is(tokenizer::TYPE_SYMBOL))
        {
            return -1;
        }

        unsigned int el = getOperatorValue(current.getValue());

        switch (el)
        {
        case consts::AND:
        case consts::OR:
        case consts::GREATER_THEN_OR_EQUAL:
        case consts::LESS_THEN_OR_EQUAL:
        case consts::GREATER_THEN:
        case consts::EQUAL_EQUAL:
        case consts::LESS_THEN:
            return 10;
        case consts::PLUS:
            return 20;
        case consts::MINUS:
            return 30;
        case consts::MULT:
            return 40;
        case consts::DIV:
            return 50;
        default:
            return -1;
        }
    }

    Node *Parser::ParseStatement()
    {
        if (is(tokenizer::TYPE_NUMBER))
        {
            double value = stod(current.getValue());
            consume();
            return new NumericLiteral(value);
        }
        if (is(tokenizer::TYPE_IDENTIFER))
        {
            std::string value = current.getValue();
            consume();

            return new Identifier(value);
        }

        if (is(tokenizer::TYPE_STRING))
        {
            std::string value = current.getValue();
            consume();
            return new StringLiteral(value);
        }

        return nullptr;
    }

    Node *Parser::BinOpRHS(int experPrec, Node *lhs)
    {

        while (true)
        {
            int prec = getBinopPrecedence();

            if (prec < experPrec)
                return lhs;

            unsigned int op = getOperatorValue(current.getValue());
            consume();

            auto rhs = ParseStatement();
            if (rhs == nullptr)
                return nullptr;

            int nextPrec = getBinopPrecedence();

            if (prec < nextPrec)
            {
                rhs = BinOpRHS(prec + 1, std::move(rhs));
                if (!rhs)
                    throw std::logic_error("RHS of expression is invalid.");
            }

            lhs = new BinaryExpression(op, lhs, rhs);
        }

        return nullptr;
    }

    Node *Parser::ParseExpression()
    {
        auto lhs = ParseStatement();

        if (auto *name = dynamic_cast<Identifier *>(lhs); name != nullptr && is(tokenizer::TYPE_SYMBOL, '('))
        {
            consume(); // eat '('

            bool first = true;
            std::vector<Node *> arguments;
            while (!is(tokenizer::TYPE_SYMBOL, ')'))
            {
                if (!first)
                {
                    if (!is(tokenizer::TYPE_SYMBOL, ','))
                        throw std::logic_error("Expexted to find ','");
                    consume();
                }

                auto *expr = ParseExpression();

                arguments.push_back(expr);

                first = false;
            }
            consume(); // eat ')'

            lhs = new CallExpression(name, arguments);
        }

        if (lhs == nullptr)
            return nullptr;

        if (is_any("+-*<>/") || is(tokenizer::TYPE_SYMBOL, "==") || is(tokenizer::TYPE_SYMBOL, ">=") || is(tokenizer::TYPE_SYMBOL, "<=") || is(tokenizer::TYPE_SYMBOL, "!=") || is(tokenizer::TYPE_SYMBOL, "&&") || is(tokenizer::TYPE_SYMBOL, "||"))
        {
            lhs = BinOpRHS(0, lhs);
        }

        return lhs;
    }

    IfStatement *Parser::ParseIfStatement()
    {
        consume(); // eat 'if'
        if (!is(tokenizer::TYPE_SYMBOL, '('))
            throw std::logic_error("Expected to find '('");
        consume(); // eat '('

        Node *condition = ParseExpression();

        if (!is(tokenizer::TYPE_SYMBOL, ')'))
            throw std::logic_error("Expected to find ')'");
        consume(); // eat ')'

        std::vector<Node *> block = std::vector<Node *>();
        ParseBlock(block, true);

        Block *thenBlock = new Block(block);

        // else block
        if (is(tokenizer::TYPE_IDENTIFER, "else"))
        {
            consume(); // eat 'else'

            // handle if else.
            if (is(tokenizer::TYPE_IDENTIFER, "if"))
            {
                IfStatement *ifelse = ParseIfStatement();
                return new IfStatement(condition, thenBlock, ifelse);
            }

            std::vector<Node *> elseBlockList = std::vector<Node *>();
            ParseBlock(block, true);
            Block *elseBlock = new Block(block);
            return new IfStatement(condition, thenBlock, elseBlock);
        }

        return new IfStatement(condition, thenBlock, nullptr);
    }

    FunctionDeclartion *Parser::ParseFunctionDeclartion()
    {
        // IDENTIFER(fn) IDENTIFER(????) SYMBOL('(') arguments SYMBOL(')') block
        consume(); // eat 'fn'
        Identifier *name = dynamic_cast<Identifier *>(ParseStatement());
        if (name == nullptr)
            throw std::logic_error("Expected to find identifier;");

        std::vector<Parameter *> parameters;
        if (!is(tokenizer::TYPE_SYMBOL, '('))
            throw std::logic_error("Expected to find '(';");

        consume(); // eat '('
        bool first = true;
        while (!is(tokenizer::TYPE_SYMBOL, ')'))
        {
            if (!first)
            {
                if (!is(tokenizer::TYPE_SYMBOL, ','))
                    throw std::logic_error("Expected to find ','");
                consume();
            }
            first = false;

            Identifier *param = dynamic_cast<Identifier *>(ParseStatement());
            if (param == nullptr)
                throw std::logic_error("Expected to find identifier");

            if (!is(tokenizer::TYPE_SYMBOL, ':'))
                throw std::logic_error("Expected to find ':'");
            consume(); // eat ':'

            // parse typedata
            Identifier *typedata = dynamic_cast<Identifier *>(ParseStatement());
            if (typedata == nullptr)
                throw std::logic_error("Expected to find identifier");

            parameters.push_back(new Parameter(param, typedata));
            // parse function arguments
        }
        consume(); // eat ')'

        std::vector<Node *> body = std::vector<Node *>();
        ParseBlock(body, true);

        auto block = new Block(body);

        return new FunctionDeclartion(name, parameters, block);
    }

    VariableStatement *Parser::ParseVaraibleStatement()
    {
        // (IDENTIFER(let) SYMBOL(:) IDENTIFER(string|number) SYMBOL(=) expression)(,+) SYMBOL(;)
        // I.E let a: string = "", b: number = 1;

        consume(); // eat 'let'
        std::vector<VariableDeclaration *> declarations;

        bool first = true;

        while (!is(tokenizer::TYPE_SYMBOL, ';'))
        {
            if (!first)
            {
                if (!is(tokenizer::TYPE_SYMBOL, ","))
                    throw std::logic_error("Expected to find ','");
                consume(); // eat ','
            }

            Identifier *d = dynamic_cast<Identifier *>(ParseStatement());
            if (d == nullptr)
            {
                throw std::logic_error("Expected to find identifier");
            }

            if (!is(tokenizer::TYPE_SYMBOL, ':'))
                throw std::logic_error("Expected to find ':'");
            consume(); // eat ':'
            Identifier *td = dynamic_cast<Identifier *>(ParseStatement());

            if (!is(tokenizer::TYPE_SYMBOL, '='))
            {
                // variable with no initializer
                declarations.push_back(new VariableDeclaration(d, td, nullptr));
                first = false;

                continue;
            }

            consume(); // eat '='

            Node *expr = ParseExpression();

            declarations.push_back(new VariableDeclaration(d, td, expr));
            first = false;
        }

        if (!is(tokenizer::TYPE_SYMBOL, ';'))
            throw std::logic_error("Expected to find ';'");
        consume(); // eat ';'

        return new VariableStatement(declarations);
    }

    bool Parser::ParseBlock(std::vector<Node *> &statements, bool requireBrackets)
    {
        if (requireBrackets)
        {
            if (!is(tokenizer::TYPE_SYMBOL, '{'))
                throw std::logic_error("Expected to find '{'");
            consume(); // eat '{'
        }

        while ((!requireBrackets && !tokens->empty()) || (requireBrackets && !is(tokenizer::TYPE_SYMBOL, '}')))
        {
            // keyword parse
            if (int id = is_keyword(); id != -1)
            {
                switch (id)
                {
                case 0:
                {
                    auto func = ParseFunctionDeclartion();
                    if (func != nullptr)
                        statements.push_back(func);
                    break;
                }
                case 1:
                {
                    auto vars = ParseVaraibleStatement();
                    statements.push_back(vars);
                    break;
                }
                case 2:
                {
                    auto ifs = ParseIfStatement();
                    if (ifs != nullptr)
                        statements.push_back(ifs);
                    break;
                }
                case 3:
                {
                    consume(); // eat 'return'
                    auto expr = ParseExpression();
                    if (!is(tokenizer::TYPE_SYMBOL, ';'))
                        throw std::logic_error("Expected to find ';'");
                    consume();

                    statements.push_back(new ReturnStatement(expr));
                    break;
                }
                default:
                    throw std::logic_error("Unknown keyword");
                }

                continue;
            }

            if (auto statement = ParseExpression(); statement != nullptr)
            {
                if (!is(tokenizer::TYPE_SYMBOL, ';'))
                    throw std::logic_error("Expected to find ';'");

                consume();

                auto stmt = new ExpressionStatement(std::move(statement));

                statements.push_back(std::move(stmt));
            }
        }

        if (requireBrackets)
        {
            if (!is(tokenizer::TYPE_SYMBOL, '}'))
                throw std::logic_error("Expected to find '}'");
            consume(); // eat '}'
        }

        return true;
    }

    Program Parser::parse()
    {
        Program program = Program();
        std::vector<Node *> statments = std::vector<Node *>();

        if (ParseBlock(statments, false))
        {
            program.setStatements(statments);
        };

        return program;
    }
}