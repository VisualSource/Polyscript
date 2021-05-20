#pragma once
#include <vector>
#include <any>
#include "Token.h"
#include "NumberNode.h"
#include "MatchNode.h"

class Parser {
	public:
		Parser(std::vector<Token> tokens);
		std::any parse();
	private:
		Token& advance();
		std::any statements();
		std::any statement();
		std::any atom();
		std::any call();
		std::any power();
		std::any factor();
		std::any term();
		std::any expr();
		std::any compExpr();
		std::any arithExpr();
		std::any exprIf();
		std::any forExpr();
		std::any enumExpr();
		std::any whileExpr();
		std::any funcDef();
		std::any listExpr();
		std::any namespaceDef();
		std::any importDef();
		std::any matchExpr();
		std::any Identifer(bool checkEQ = true);
		MatchExpr makeMatchExpr();
		Token current_token;
		std::vector<Token> tokens;
		int tok_idx = 1;
};

