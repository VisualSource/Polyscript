#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "Position.h"

using namespace std;

class Lexer {
	public:
		Lexer(string text, string fn);
		vector<Token> makeTokens();
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
		string text;
		string fn;
		Position pos;
		char current_char = '\0';
};

