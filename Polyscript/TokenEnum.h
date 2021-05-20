#pragma once
#include <string>

enum class TypeToken {
	DIV, // '/' Division
	MUL, // '*' Multiplication
	PLUS, // '+': Addition
	MINUS, // '-' Subtraction, Negation
	POWER, // '^'
	INT,
	FLOAT,
	STRING,
	LPAREN, // '('
	RPAREN, // ')'
	LBRACKET, // '['
	RBRACKET, // ']'
	IDENTIFIER,
	KEYWORD,
	CONDITIONAL, // ':' Various separators TODO: rename Colon
	ENDSTATEMENT, // ';' TODO rename 'Semi' Terminator for various items and statements, Array types
	SCOPESTART,
	PATHSEP, // '::' Path separator
	SCOPEEND, // '}'
	NOT, // '!'
	AND, // '&&'
	OR, // '||'
	EQ, // '='
	COMMA, // ','
	RARROW, // Function return type, Closure return type, Function pointer type
	FATARROW, // Match arms, Macros
	EE, // ==
	NE, // !=
	LT, // <
	GT, // >
	LTE, // <=,
	GTE, // >=
	T_EOF, // '\0'
	T_UNDEFINED,
	NEWLINE,
	PLUS_EQ,
	MINUS_EQ,
	DIV_EQ,
	MUL_EQ,
	PLUS_PLUS,
	MINUS_MINUS,
	DOT,
};

namespace TokenUtils {
	std::string printTokenType(TypeToken token);
}
