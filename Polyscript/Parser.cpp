#include "Parser.h"
#include <optional>
#include "UnaryOpNode.h"
#include "IfNode.h"
#include "NodeUtils.h"
#include "InvalidSyntaxError.h"
#include "ForNode.h"
#include "WhileNode.h"
#include "VarAccessNode.h"
#include "VarAssignNode.h"
#include "NumberNode.h"
#include "UnaryOpNode.h"
#include "BinOpNode.h"


Parser::Parser(vector<Token> tokens): tokens(tokens) {
	tok_idx = -1;
	advance();
}

Token& Parser::advance() {
	tok_idx++;
	if(tok_idx < (int)tokens.size()){
		current_token = tokens.at(tok_idx);
	}
 	return current_token;
}
ParseResult Parser::atom(){
	ParseResult res;
	Token tok = current_token;

	if (tok.isToken(TypeToken::INT) || tok.isToken(TypeToken::FLOAT)) {
		res.Register(advance());
		return res.success(NumberNode(tok));
	}
	else if (tok.isToken(TypeToken::IDENTIFIER)) {
		res.Register(advance());
		return res.success(VarAccessNode(tok));
	}
	else if (tok.isToken(TypeToken::LPAREN)) {
		res.Register(advance());
		any expr = res.Register(this->expr());
		if (res.hasErr()) throw res.GetErr();
		if (current_token.isToken(TypeToken::RPAREN)) {
			res.Register(advance());
			return res.success(expr);
		} else {
			throw InvalidSyntaxError("Expected ')'", current_token.GetStart().value(), current_token.GetEnd().value());
		}
	} else if (tok.matches(TypeToken::KEYWORD,"if")){
		any expr_if = res.Register(exprIf());
		return res.success(expr_if);
	}
	else if (tok.matches(TypeToken::KEYWORD, "for")) {
		any expr_if = res.Register(forExpr());
		return res.success(expr_if);
	}
	else if (tok.matches(TypeToken::KEYWORD, "while")) {
		any expr_if = res.Register(whileExpr());
		return res.success(expr_if);
	}

	throw InvalidSyntaxError("Expected int or float, '+', '-', or '(' ", tok.GetStart().value(), tok.GetEnd().value());
}
ParseResult Parser::power() {
	ParseResult res;
	any left = res.Register(atom());
	if (res.hasErr()) throw res.GetErr();

	while (current_token.isToken(TypeToken::POWER)) {
		Token op_tok = current_token;
		res.Register(advance());
		any right = res.Register(factor());
		if (res.hasErr()) throw res.GetErr();
		left = BinOpNode(left, op_tok, right);
	}
	return res.success(left);
}
ParseResult Parser::parse() {
	ParseResult res = expr();
	if (!res.hasErr() && !current_token.isToken(TypeToken::T_EOF)) {
		throw InvalidSyntaxError("Expected ;", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	return res;
}

ParseResult Parser::factor(){
	ParseResult res;
	Token tok = current_token;

	if (tok.isToken(TypeToken::PLUS) || tok.isToken(TypeToken::MINUS)) {
		res.Register(advance());
		any factor = res.Register(this->factor());
		if (res.hasErr()) throw res.GetErr();
		return res.success(UnaryOpNode(tok, factor));
	} 

	return power();
}

ParseResult Parser::term(){

	ParseResult res;
	any left = res.Register(factor());
	if (res.hasErr()) throw res.GetErr();

	while (current_token.isToken(TypeToken::MUL) || current_token.isToken(TypeToken::DIV)) {
		Token op_tok = current_token;
		res.Register(advance());
		any right = res.Register(factor());
		if (res.hasErr()) throw res.GetErr();
		left = BinOpNode(left,op_tok,right);
	}
	return res.success(left);
}

ParseResult Parser::expr() {
	ParseResult res;
	if (current_token.matches(TypeToken::KEYWORD, "let")) {
		res.Register(advance());

		if (!current_token.isToken(TypeToken::IDENTIFIER)) {
			throw InvalidSyntaxError("Expected identifier", current_token.GetStart().value(), current_token.GetEnd().value());
		}
		Token varName = current_token;
		res.Register(advance());

		optional<Token> varType = nullopt;
		if (current_token.isToken(TypeToken::CONDITIONAL)) {
			res.Register(advance());
			if (current_token.matches(TypeToken::KEYWORD, "int") || current_token.matches(TypeToken::KEYWORD, "float")) {
				varType = current_token;
				res.Register(advance());
			} else {
				throw InvalidSyntaxError("Expected Type identifier", current_token.GetStart().value(), current_token.GetEnd().value());
			}
		}

		if (!current_token.isToken(TypeToken::EQ)) {
			throw InvalidSyntaxError("Expected assigment", current_token.GetStart().value(), current_token.GetEnd().value());
		}
		res.Register(advance());

		any expr = res.Register(this->expr());
		if (res.hasErr()) {
			throw res.GetErr();
		}

		return res.success(VarAssignNode(varName,expr,varType));
	}


	
	any left = res.Register(compExpr());
	if (res.hasErr()) throw res.GetErr();
	while (current_token.isToken(TypeToken::AND) || current_token.isToken(TypeToken::OR)) {
		Token op_tok = current_token;
		res.Register(advance());
		any right = res.Register(compExpr());
		if (res.hasErr()) throw res.GetErr();
		left = BinOpNode(left, op_tok, right);
	}
	return res.success(left);
}

ParseResult Parser::compExpr()
{
	ParseResult res;

	if (current_token.isToken(TypeToken::NOT)) {
		Token op_tok = current_token;
		res.Register(advance());

		any node = res.Register(compExpr());
		if (res.hasErr()) throw res.GetErr();
		return res.success(UnaryOpNode(op_tok,node));
	}

	any left = res.Register(arithExpr());
	if (res.hasErr()) throw res.GetErr();

	while (current_token.isToken(TypeToken::EE) || current_token.isToken(TypeToken::NE) || current_token.isToken(TypeToken::LT) 
		|| current_token.isToken(TypeToken::GT) || current_token.isToken(TypeToken::LTE) || current_token.isToken(TypeToken::GTE)) {
		Token op_tok = current_token;
		res.Register(advance());
		any right = res.Register(arithExpr());
		if (res.hasErr()) throw res.GetErr();
		left = BinOpNode(left, op_tok, right);
	}

	return res.success(left);
}

ParseResult Parser::arithExpr()
{
	ParseResult res;
	any left = res.Register(term());
	if (res.hasErr()) throw res.GetErr();

	while (current_token.isToken(TypeToken::PLUS) || current_token.isToken(TypeToken::MINUS)){
		Token op_tok = current_token;
		res.Register(advance());
		any right = res.Register(term());
		if (res.hasErr()) throw res.GetErr();
		left = BinOpNode(left, op_tok, right);
	}

	return res.success(left);
}


ParseResult Parser::exprIf()
{
	vector<IfCases> cases;
	ParseResult res;
	any else_case;

	if (!current_token.matches(TypeToken::KEYWORD, "if")) {
		throw InvalidSyntaxError("Expexted 'if'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any condition = res.Register(expr());

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'",current_token.GetStart().value(),current_token.GetEnd().value());
	}

	advance();

	any expra = res.Register(this->expr());
	if (expra.has_value()) {
		cases.push_back(IfCases{ condition, expra });
	}
	else {
		advance();
	}
	

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	advance();

	while (current_token.matches(TypeToken::KEYWORD, "else")) {
		advance();
		if (current_token.matches(TypeToken::KEYWORD, "if")) {
			advance();
			any condition = res.Register(expr());
			if (!current_token.isToken(TypeToken::SCOPESTART)) {
				throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			advance();
			any expra = res.Register(this->expr());
			if (expra.has_value()) {
				cases.push_back(IfCases{ condition, expra });
			}
			else {
				advance();
			}

			if (!current_token.isToken(TypeToken::SCOPEEND)) {
				throw InvalidSyntaxError("Expected '}'", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			advance();
		}
		else if (current_token.isToken(TypeToken::SCOPESTART)) {
			advance();
			else_case = res.Register(this->expr());

			if (!current_token.isToken(TypeToken::SCOPEEND)) {
				throw InvalidSyntaxError("Expected '}'", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			advance();
		}
		else {
			throw InvalidSyntaxError("Expected 'if' or '{'",current_token.GetStart().value(),current_token.GetEnd().value());
		}
		
	}


	return res.success(IfNode(cases, else_case));
}

// Creation of a for loop block
// Grammer
// for IDENTIFIER = NUMBER to NUMBER { EXPR }
ParseResult Parser::forExpr()
{
	ParseResult res;
	
	if (!current_token.matches(TypeToken::KEYWORD, "for")) {
		throw InvalidSyntaxError("Expected 'for'",current_token.GetStart().value(),current_token.GetEnd().value());
	}

	advance();

	if(!current_token.isToken(TypeToken::IDENTIFIER)){
		throw InvalidSyntaxError("Expected identifier", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	Token varName = current_token;
	advance();

	if (!current_token.isToken(TypeToken::EQ)) {
		throw InvalidSyntaxError("Expected '='",current_token.GetStart().value(),current_token.GetEnd().value());
	}
	advance();

	any start_value = res.Register(this->expr());

	if (!current_token.matches(TypeToken::KEYWORD, "to")) {
		throw InvalidSyntaxError("Expected 'to'",current_token.GetStart().value(),current_token.GetEnd().value());
	}

	advance();

	any end_value = res.Register(this->expr());

	any step_value;
	if (current_token.matches(TypeToken::KEYWORD, "step")) {
		advance();

		step_value = res.Register(this->expr());
	}

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any body = res.Register(this->expr());


	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();


	return res.success(ForNode(varName,start_value,end_value,body,step_value));
}

// Grammer
// while <condition> { expr }
ParseResult Parser::whileExpr()
{
	ParseResult res;

	if(!current_token.matches(TypeToken::KEYWORD,"while")){
		throw InvalidSyntaxError("Expected 'while'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any condition = res.Register(this->expr());

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any body = res.Register(this->expr());

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	return res.success(WhileNode(condition,body));
}


