#pragma once

namespace ast
{
    namespace consts
    {
        const unsigned int NOT = '!';
        const unsigned int EQUAL = '=';
        const unsigned int MINUS = '-';
        const unsigned int PLUS = '+';
        const unsigned int DIV = '/';
        const unsigned int MULT = '*';
        const unsigned int AND = '&' + '&';
        const unsigned int OR = '|' + '|';
        const unsigned int LESS_THEN = '<';
        const unsigned int GREATER_THEN = '>';
        const unsigned int LESS_THEN_OR_EQUAL = LESS_THEN + '=';
        const unsigned int GREATER_THEN_OR_EQUAL = GREATER_THEN + '=';
        const unsigned int EQUAL_EQUAL = EQUAL + EQUAL;
        const unsigned int NOT_EQUAL = NOT + EQUAL;
        const unsigned int SET_MINUS = MINUS + EQUAL;
        const unsigned int SET_PLUS = PLUS + EQUAL;

        const unsigned int BINARY_EXPRESSION = 1;
        const unsigned int BLOCK_EXPRESSION = 2;
        const unsigned int CALL_EXPRESSION = 3;
        const unsigned int EXPRESSION_STATEMENT = 4;
        const unsigned int FUNCTION_EXPRESSION = 5;
        const unsigned int IDENTIFIER = 6;
        const unsigned int IF_STATEMENT = 7;
        const unsigned int NUMBERIC_LITERAL = 8;
        const unsigned int RETURN_STATEMENT = 9;
        const unsigned int VARIABLE_DECLARATION = 10;
        const unsigned int VARIABLE_STATEMENT = 11;
        const unsigned int STRING_LITERAL = 12;
        const unsigned int PARAMETER_EXRESSION = 13;
        const unsigned int WHILE_EXRESSION = 14;

    } // namespace consts

} // namespace ast
