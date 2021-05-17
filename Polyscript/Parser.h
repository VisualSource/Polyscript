#pragma once
#include <vector>
#include <any>
#include "Token.h"
#include "NumberNode.h"
#include "MatchNode.h"

using namespace std;


class Parser {
	public:
		Parser(vector<Token> tokens);
		any parse();
	private:
		Token& advance();
		any statements();
		any statement();
		any atom();
		any call();
		any power();
		any factor();
		any term();
		any expr();
		any compExpr();
		any arithExpr();
		any exprIf();
		any forExpr();
		any enumExpr();
		any whileExpr();
		any funcDef();
		any listExpr();
		any namespaceDef();
		any importDef();
		any matchExpr();
		any Identifer(bool checkEQ = true);
		MatchExpr makeMatchExpr();
		Token current_token;
		vector<Token> tokens;
		int tok_idx = 1;
};

