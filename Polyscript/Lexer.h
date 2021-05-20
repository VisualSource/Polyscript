#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "Position.h"

class Lexer {
	public:
		Lexer(std::string text, std::string fn);
		std::vector<Token> makeTokens();
	private:
		void advance();
		Token makeNumber();
		Token makeIndentifier();
		Token makeEquals();
		Token makeNotEquals();
		Token makeLessThan();
		Token makeGreaterThan();
		Token makeLogicOR();
		Token makeLogicAND();
		Token MakeMinusORRArrow();
		Token makeString();
		void MakeComment();
		std::string text;
		std::string fn;
		Position pos;
		char current_char = '\0';
};

