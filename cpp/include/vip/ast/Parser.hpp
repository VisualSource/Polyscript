#pragma once
#include <deque>
#include "./FunctionDeclaration.hpp"
#include "./VariableStatement.hpp"
#include "../tokenizer/Token.hpp"
#include "./IfStatement.hpp"
#include "./Program.hpp"

namespace ast
{
    class Parser
    {
    private:
        std::deque<tokenizer::Token> *tokens;
        tokenizer::Token current;
        /// @brief pops the first item in the deque and assigns it to the current prop.
        void consume();
        /// @brief Check if the next otken will be of the given type.
        /// @param type The type of token
        /// @return
        bool is_next(unsigned int type);
        /// @brief Check if the next token will be the the given type and contain a value of the given char.
        /// @param type The type of token
        /// @param value the char the the token should have.
        /// @return
        bool is_next(unsigned int type, char value);
        /// @brief check if the next token will contain a char in the given cstring
        /// @param check a cstring of the chars that the token may contain.
        /// @return
        bool is_next(const char *check);
        /// @brief check if the current token is of the give type.
        /// @param type type of the token
        /// @return
        bool is(unsigned int type);
        /// @brief checks if the current token is of the given type and has contains the given char.
        /// @param type type of the token
        /// @param value the char that the token should contain.
        /// @return
        bool is(unsigned int type, char value);
        /// @brief check if the current token if of the given type and have the value of the given string.
        /// @param type type of the token
        /// @param value the value that the token should have
        /// @return
        bool is(unsigned int type, std::string value);
        /// @brief check if the current token is in the given cstring.
        /// @param check
        /// @return
        bool is_any(const char *check);
        /// @brief check if current token is a keyword, if so return a int that is assoceded with that key word else -1.
        /// @return
        int is_keyword();
        /// @brief get precedence of the current token.
        /// @return the precedence of the token.
        int getBinopPrecedence();

        Node *main();
        /// @brief parse a block of statements.
        /// @param statements the list to append statements to.
        /// @param includeBrackets should this block be wrapped in '{' and '}' tokens. Default is false
        /// @return was the operation successful
        bool ParseBlock(std::vector<Node *> &statements, bool includeBrackets = false);
        /// @brief parse values literials and identifers
        /// @return pointer to statement
        Node *ParseStatement();
        /// @brief parse a binary expression
        /// @param experPrec the precedence of previous expression.
        /// @param lhs expression
        /// @return expression or binary expression.
        Node *BinOpRHS(int experPrec, Node *lhs);
        /// @brief parse a expression
        /// @return
        Node *ParseExpression();
        /// @brief parses a variable statement
        /// @return variable statement
        VariableStatement *ParseVaraibleStatement();
        /// @brief parses a function declartion
        /// @return function declartion
        FunctionDeclartion *ParseFunctionDeclartion();
        /// @brief parses a if statement.
        /// @return if statement
        IfStatement *ParseIfStatement();

    public:
        Parser(std::deque<tokenizer::Token> *tokens);
        /// @brief Parse tokens from tokienizer into ast
        /// @return
        Program parse();
    };
} // namespace ast
