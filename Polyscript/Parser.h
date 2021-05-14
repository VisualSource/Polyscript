#pragma once
#include <vector>
#include <any>
#include "Token.h"
#include "ParseResult.h"
#include "NumberNode.h"

using namespace std;

class Parser {
	public:
		Parser(vector<Token> tokens);
		ParseResult parse();
	private:
		Token& advance();
		ParseResult atom();
		ParseResult power();
		ParseResult factor();
		ParseResult term();
		ParseResult expr();
		ParseResult compExpr();
		ParseResult arithExpr();
		ParseResult exprIf();
		Token current_token;
		vector<Token> tokens;
		int tok_idx = 1;
};

