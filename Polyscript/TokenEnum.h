#pragma once
#include <string>

using namespace std;

enum class TypeToken {
	DIV,
	MUL,
	PLUS,
	MINUS,
	POWER,
	INT,
	FLOAT,
	LPAREN,
	RPAREN,
	IDENTIFIER,
	KEYWORD,
	CONDITIONAL,
	ENDSTATEMENT,
	SCOPESTART,
	SCOPEEND,
	NOT,
	AND,
	OR,
	EQ,
	EE, // ==
	NE, // !=
	LT, // <
	GT, // >
	LTE, // <=,
	GTE, // >=
	T_EOF,
	T_UNDEFINED
};

namespace TokenUtils {
	string printTokenType(TypeToken token);
}
