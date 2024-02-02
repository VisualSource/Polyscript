#pragma once
#include <deque>
#include "./Program.hpp"
#include "../tokenizer/Token.hpp"

namespace ast
{
    class Parser
    {
    private:
        std::deque<Token> *tokens;
        Token current;

        void consume();
        bool is_next(unsigned int type);
        bool is_next(unsigned int type, char value);
        bool is_next(const char *check);
        bool is(unsigned int type);
        bool is(unsigned int type, char value);
        bool is_any(const char *check);
        bool is_keyword();

        int getBinopPrecedence();

        Node *main();

        bool ParseBlock(std::vector<Node *> &statements, bool includeBrackets = false);
        Node *ParseStatement();
        Node *BinOpRHS(int experPrec, Node *lhs);
        Node *ParseExpressionStatement();
        Node *ParseBinaryExpression(int expresionWeight, Node *lhs);
        Node *ParseNumericLiteral();

    public:
        Parser(std::deque<Token> *tokens);
        Program parse();
    };
} // namespace ast
