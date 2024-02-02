#include "./Parser.hpp"
#include "./NumericLiteral.hpp"
#include "./Identifier.hpp"
#include "./ExpressionStatement.hpp"
#include "./BinaryExpression.hpp"
#include <string>
#include <string.h>

namespace ast
{

    Parser::Parser(std::deque<Token> *tokens) : tokens(tokens), current(Token())
    {
        consume();
    }

    void Parser::consume()
    {
        if (tokens->empty())
        {
            return;
        }
        current = Token(tokens->front());
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
        if (ref.getType() != TYPE_SYMBOLE)
            return false;

        return strchr(check, ref.getValue().at(0)) != nullptr;
    }
    bool Parser::is_keyword()
    {
        auto value = current.getValue();
        return is(TYPE_IDENTIFER) && (value == "fn" || value == "let" || value == "if");
    }
    bool Parser::is_any(const char *check)
    {
        if (current.getType() != TYPE_SYMBOLE)
            return false;
        return strchr(check, current.getValue().at(0)) != nullptr;
    }

    int Parser::getBinopPrecedence()
    {
        if (!is(TYPE_SYMBOLE))
        {
            return -1;
        }

        char el = current.getValue().at(0);

        switch (el)
        {
        case '<':
            return 10;
        case '+':
            return 20;
        case '-':
            return 30;
        case '*':
            return 40;
        default:
            return -1;
        }
    }

    Node *Parser::ParseStatement()
    {

        if (is(TYPE_NUMBER))
        {
            double value = stod(current.getValue());
            consume();
            return new NumericLiteral(value);
        }
        if (is(TYPE_IDENTIFER))
        {
            std::string value = current.getValue();
            if (is_next(TYPE_SYMBOLE, '('))
            {
                consume(); // eat '('

                // parse args,
                // ParseStatement

                if (!is(TYPE_SYMBOLE, ')'))
                {
                    return nullptr;
                }
                consume(); // eat ')'

                // return function call;
            }

            return new Identifier(value);
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

            char op = current.getValue().at(0);
            consume();

            auto rhs = ParseStatement();
            if (rhs == nullptr)
                return nullptr;

            int nextPrec = getBinopPrecedence();

            if (prec < nextPrec)
            {
                rhs = BinOpRHS(prec + 1, std::move(rhs));
                if (!rhs)
                    return nullptr;
            }

            lhs = new BinaryExpression(op, lhs, rhs);
        }

        return nullptr;
    }

    Node *Parser::ParseExpressionStatement()
    {
        auto lhs = ParseStatement();

        if (lhs == nullptr)
            return nullptr;

        if (is_any("+-*>"))
        {
            lhs = BinOpRHS(0, lhs);
        }

        if (!is(TYPE_SYMBOLE, ';'))
        {
            return nullptr;
        }

        return new ExpressionStatement(std::move(lhs));
    }

    bool Parser::ParseBlock(std::vector<Node *> &statements, bool requireBrackets)
    {
        if (requireBrackets && !is(TYPE_SYMBOLE, '{'))
            return false;

        // keyword parse
        if (is_keyword())
        {
        }
        else
        {
            auto statement = ParseExpressionStatement();
            if (statement != nullptr)
            {

                statements.push_back(std::move(statement));
            }
        }

        if (requireBrackets && !is(TYPE_SYMBOLE, '}'))
            return false;
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