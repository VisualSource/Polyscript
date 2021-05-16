#include "Parser.h"
#include <optional>
#include "Nodes.h"
#include "NodeUtils.h"
#include "InvalidSyntaxError.h"
#include "TokenEnum.h"


Parser::Parser(vector<Token> tokens): tokens(tokens) {
	tok_idx = -1;
	advance();
}

any Parser::parse() {
	any res = statements();
	if (!current_token.isToken(TypeToken::T_EOF)) {
		throw InvalidSyntaxError("Expected ;", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	return res;
}

Token& Parser::advance() {
	tok_idx++;
	if(tok_idx < (int)tokens.size()){
		current_token = tokens.at(tok_idx);
	}
 	return current_token;
}
any Parser::statements()
{
	vector<any> statements;
	Position start(current_token.GetStart().value());

	while (current_token.isToken(TypeToken::NEWLINE)){
		advance();
	}

	statements.push_back(this->statement());

	bool more_statments = true;
	
	while (true) {
		int newlines = 0;
		while (current_token.isToken(TypeToken::NEWLINE) || current_token.isToken(TypeToken::ENDSTATEMENT)) {
			advance();
			newlines++;
		}
		if (newlines == 0) {
			more_statments = false;
		}
		if (!more_statments) break;
		int reset = tok_idx;
		any statement = this->statement();
		if (!statement.has_value()) {
			tok_idx = reset;
			current_token = tokens.at(tok_idx);
			more_statments = false;
			continue;
		}
		
		statements.push_back(statement);
		

	}

	return ListNode(statements,start,current_token.GetEnd().value());
}

any Parser::statement()
{
	Position start(current_token.GetStart().value());

	if (current_token.matches(TypeToken::KEYWORD, "return")) {
		advance();

		int reset = tok_idx;
		any statement = this->expr();
		if (!statement.has_value()) {
			tok_idx = reset;
			current_token = tokens.at(tok_idx);
		}
		return ReturnNode(statement, start, current_token.GetEnd().value());

	}
	if (current_token.matches(TypeToken::KEYWORD, "continue")) {
		advance();
		return ContinueNode(start, current_token.GetEnd().value());
	}
	if (current_token.matches(TypeToken::KEYWORD, "break")) {
		advance();
		return BreakNode(start, current_token.GetEnd().value());
	}

	return this->expr();
}
any Parser::atom(){
	Token tok = current_token;
	Position start(tok.GetStart().value());

	if (tok.isToken(TypeToken::INT) || tok.isToken(TypeToken::FLOAT)) {
		advance();
		return NumberNode(tok);
	}
	else if (tok.isToken(TypeToken::STRING)) {
		advance();
		return StringNode(tok);
	}
	else if (tok.isToken(TypeToken::IDENTIFIER)) {
		advance();

		if(current_token.isToken(TypeToken::LBRACKET)){
			advance();

			any index = this->expr();

			if (!current_token.isToken(TypeToken::RBRACKET)) {
				throw InvalidSyntaxError("Expected ']'", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			advance();

			return ListAccessNode(tok, index,start,current_token.GetEnd().value());
		
		}
		else if (current_token.isToken(TypeToken::PATHSEP)) {
			vector<Token> path;
			path.push_back(tok);

			while (current_token.isToken(TypeToken::PATHSEP))
			{
				advance();
				if (!current_token.isToken(TypeToken::IDENTIFIER)) {
					throw InvalidSyntaxError("Expected IDENTIFIER", current_token.GetStart().value(), current_token.GetEnd().value());
				}
				path.push_back(current_token);
				advance();
			}

			//advance();

			return PathAccessNode(path,start,current_token.GetEnd().value());
		}
		else if(current_token.isToken(TypeToken::PLUS_EQ) || current_token.isToken(TypeToken::MINUS_EQ) 
				|| current_token.isToken(TypeToken::MUL_EQ) || current_token.isToken(TypeToken::DIV_EQ) || current_token.isToken(TypeToken::EQ)){
			Token op = current_token;
			advance();

			any value = this->expr();

			return VarReasignNode(tok,op,value,start,current_token.GetEnd().value());

		}

		return VarAccessNode(tok);
	}
	else if (tok.isToken(TypeToken::LPAREN)) {
		advance();
		any expr = this->expr();
		if (current_token.isToken(TypeToken::RPAREN)) {
			advance();
			return expr;
		} else {
			throw InvalidSyntaxError("Expected ')'", current_token.GetStart().value(), current_token.GetEnd().value());
		}
	}
	else if (tok.isToken(TypeToken::LBRACKET)) {
		return listExpr();
	}
	else if (tok.matches(TypeToken::KEYWORD,"if")){
		return exprIf();
	}
	else if (tok.matches(TypeToken::KEYWORD, "for")) {
		return forExpr();
	}
	else if (tok.matches(TypeToken::KEYWORD, "while")) {
		return whileExpr();
	}
	else if(tok.matches(TypeToken::KEYWORD,"fn")){
		return funcDef();
	}
	else if (tok.matchesKeyWord("enum")) {
		return enumExpr();
	}

	return any();
}

/*
Function call;
atom (LPAREN (IDENTIFIER (COMMA IDENTIFIER)*)? RPAREN)?
*/
any Parser::call()
{
	any atom = this->atom();

	if (current_token.isToken(TypeToken::LPAREN)) {
		advance();
		vector<any> arg_nodes;
		if (current_token.isToken(TypeToken::RPAREN)) {
			advance();
		}
		else {
			try{
				arg_nodes.push_back(this->expr());
			}
			catch (InvalidSyntaxError){
				throw InvalidSyntaxError("Expected ')' 'let', 'if', 'for', 'while', 'fn', int, float, identifier" , current_token.GetStart().value(), current_token.GetEnd().value());
			}
			while (current_token.isToken(TypeToken::COMMA)) {
				advance();
				arg_nodes.push_back(this->expr());
			}
			if (!current_token.isToken(TypeToken::RPAREN)) {
				throw InvalidSyntaxError("Expected identifier or ')'", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			advance();
		}
		return FuncCallNode(atom,arg_nodes);
	}

	
	return atom;
}
any Parser::power() {
	any left = this->call();

	while (current_token.isToken(TypeToken::POWER)) {
		Token op_tok = current_token;
		advance();
		any right = factor();
		left = BinOpNode(left, op_tok, right);
	}
	return left;
}

any Parser::factor(){
	Token tok = current_token;

	if (tok.isToken(TypeToken::PLUS) || tok.isToken(TypeToken::MINUS)) {
		advance();
		any factor = this->factor();
		return UnaryOpNode(tok, factor);
	} 

	return power();
}

any Parser::term(){

	any left = factor();

	while (current_token.isToken(TypeToken::MUL) || current_token.isToken(TypeToken::DIV)) {
		Token op_tok = current_token;
		advance();
		any right = factor();
		left = BinOpNode(left,op_tok,right);
	}
	return left;
}


/*
 Creation of 

 KEYWORD:let IDENETIFER (CONDIONAL KEYWORD)? EQ expr
 AND|OR

*/
any Parser::expr() {
	if (current_token.matches(TypeToken::KEYWORD, "let")) {
		advance();

		if (!current_token.isToken(TypeToken::IDENTIFIER)) {
			throw InvalidSyntaxError("Expected identifier", current_token.GetStart().value(), current_token.GetEnd().value());
		}
		Token varName = current_token;
		advance();

		optional<Token> varType = nullopt;
		if (current_token.isToken(TypeToken::CONDITIONAL)) {
			advance();
			if (current_token.matches(TypeToken::KEYWORD, "int") || current_token.matches(TypeToken::KEYWORD, "float") || current_token.matches(TypeToken::KEYWORD, "string") || current_token.matches(TypeToken::KEYWORD,"bool")) {
				varType = current_token;
				advance();
			} else {
				throw InvalidSyntaxError("Expected Type identifier", current_token.GetStart().value(), current_token.GetEnd().value());
			}
		}

		if (!current_token.isToken(TypeToken::EQ)) {
			throw InvalidSyntaxError("Expected assigment", current_token.GetStart().value(), current_token.GetEnd().value());
		}
		advance();

		any expr = this->expr();
		
		return VarAssignNode(varName,expr,varType);
	}


	
	any left = compExpr();
	while (current_token.isToken(TypeToken::AND) || current_token.isToken(TypeToken::OR)) {
		Token op_tok = current_token;
		advance();
		any right = compExpr();
		left = BinOpNode(left, op_tok, right);
	}
	return left;
}

any Parser::compExpr()
{
	
	if (current_token.isToken(TypeToken::NOT)) {
		Token op_tok = current_token;
		advance();
		any node = compExpr();
		return UnaryOpNode(op_tok,node);
	}

	any left = arithExpr();

	while (current_token.isToken(TypeToken::EE) || current_token.isToken(TypeToken::NE) || current_token.isToken(TypeToken::LT) 
		|| current_token.isToken(TypeToken::GT) || current_token.isToken(TypeToken::LTE) || current_token.isToken(TypeToken::GTE)) {
		Token op_tok = current_token;
		advance();
		any right = arithExpr();
		left = BinOpNode(left, op_tok, right);
	}

	return left;
}

any Parser::arithExpr()
{
	any left = term();

	while (current_token.isToken(TypeToken::PLUS) || current_token.isToken(TypeToken::MINUS)){
		Token op_tok = current_token;
		advance();
		any right = term();
		left = BinOpNode(left, op_tok, right);
	}

	return left;
}

// Grammer KEYWORD:if expr { (expr*)? } (else (if {}) 
any Parser::exprIf()
{
	vector<IfCases> cases;
	any else_case;

	if (!current_token.matches(TypeToken::KEYWORD, "if")) {
		throw InvalidSyntaxError("Expexted 'if'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any condition = expr();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'",current_token.GetStart().value(),current_token.GetEnd().value());
	}

	advance();

	any expra = statements();
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
			any condition = expr();
			if (!current_token.isToken(TypeToken::SCOPESTART)) {
				throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			advance();
			any expra = statements();
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
			else_case = statements();

			if (!current_token.isToken(TypeToken::SCOPEEND)) {
				throw InvalidSyntaxError("Expected '}'", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			advance();
		}
		else {
			throw InvalidSyntaxError("Expected 'if' or '{'",current_token.GetStart().value(),current_token.GetEnd().value());
		}
		
	}


	return IfNode(cases, else_case);
}

// Creation of a for loop block
// Grammer: KEYWORD:for IDENTIFIER EQ expr KEYWORD:to expr SCOPESTART expr SCOPEEND
any Parser::forExpr()
{
	
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

	any start_value = this->expr();

	if (!current_token.matches(TypeToken::KEYWORD, "to")) {
		throw InvalidSyntaxError("Expected 'to'",current_token.GetStart().value(),current_token.GetEnd().value());
	}

	advance();

	any end_value = this->expr();

	any step_value;
	if (current_token.matches(TypeToken::KEYWORD, "step")) {
		advance();

		step_value = this->expr();
	}

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any body = this->statements();


	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();


	return ForNode(varName,start_value,end_value,body,step_value);
}

/*
Grammer 

KEYWORLD:enum IDENTIFIER SCOPESTART (IDENTIFER (COMMA)?)? SCOPEEND

*/
any Parser::enumExpr()
{
	Position start(current_token.GetStart().value());
	if (!current_token.matchesKeyWord("enum")) {
		throw InvalidSyntaxError("Expected keyworld 'enum'", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	advance();

	if (!current_token.isToken(TypeToken::IDENTIFIER)) {
		throw InvalidSyntaxError("Expected Identifier", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	string name = current_token.GetValue().value();
	advance();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected Identifier", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	advance();
	vector<Token> enumsKeys;
	while (!current_token.isToken(TypeToken::SCOPEEND))
	{
		if (current_token.isToken(TypeToken::NEWLINE)) {
			advance();
			continue;
		}
		else if (current_token.isToken(TypeToken::IDENTIFIER)) {
			enumsKeys.push_back(current_token);
			advance();
			if (current_token.isToken(TypeToken::COMMA)) {
				advance();
			}
		}else {
			throw InvalidSyntaxError("Expected Identifier or ','", current_token.GetStart().value(), current_token.GetEnd().value());
		}
	}

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	advance();

	return EnumNode(name,enumsKeys,start,current_token.GetEnd().value());
}

// Grammer: KEYWORD:while expr SCOPESTART expr SCOPEEND
any Parser::whileExpr()
{
	
	if(!current_token.matches(TypeToken::KEYWORD,"while")){
		throw InvalidSyntaxError("Expected 'while'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any condition = this->expr();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any body = this->statements();

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	return WhileNode(condition,body);
}

/*
	Gammer: KEYWORD:fn IDENTIFIER? LPAREN (IDENTIFIER (COMMA IDENTIFIER)*)? RPAREN (RARROW TYPE) ? SCOPESTART expr SCOPEEND
*/
any Parser::funcDef()
{
	if (!current_token.matches(TypeToken::KEYWORD, "fn")) {
		throw InvalidSyntaxError("Expected 'fn'", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	advance();
	optional<Token> var_name = nullopt;
	if (current_token.isToken(TypeToken::IDENTIFIER)) {
		var_name = current_token;
		advance();
		if (!current_token.isToken(TypeToken::LPAREN)) {
			throw InvalidSyntaxError("Expected '('", current_token.GetStart().value(), current_token.GetEnd().value());
		}
	}
	else {
		if (!current_token.isToken(TypeToken::LPAREN)) {
			throw InvalidSyntaxError("Expected identifier or '('", current_token.GetStart().value(), current_token.GetEnd().value());
		}
	}

	advance();

	vector<Token> arg_names;

	if (current_token.isToken(TypeToken::IDENTIFIER)) {
		arg_names.push_back(current_token);
		advance();
		while (current_token.isToken(TypeToken::COMMA)){
			advance();
			if (!current_token.isToken(TypeToken::IDENTIFIER)) {
				throw InvalidSyntaxError("Expected identifier", current_token.GetStart().value(), current_token.GetEnd().value());
			}
			arg_names.push_back(current_token);
			advance();
		}
		if (!current_token.isToken(TypeToken::RPAREN)) {
			throw InvalidSyntaxError("Expected ',' or ')'", current_token.GetStart().value(), current_token.GetEnd().value());
		}
	}
	else {
		if (!current_token.isToken(TypeToken::RPAREN)) {
			throw InvalidSyntaxError("Expected identifier or ')'", current_token.GetStart().value(), current_token.GetEnd().value());
		}
	}

	advance();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	any return_node = this->statements();

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart().value(), current_token.GetEnd().value());
	}

	advance();

	return FuncDefNode(var_name,arg_names,return_node);
}

any Parser::listExpr()
{
	vector<any> nodes;
	Position start(current_token.GetStart().value());

	if (!current_token.isToken(TypeToken::LBRACKET)) {
		throw InvalidSyntaxError("Expected '['", current_token.GetStart().value(), current_token.GetEnd().value());
	}
	advance();

	if (current_token.isToken(TypeToken::RBRACKET)) {
		advance();
	}
	else {
		try {
			nodes.push_back(this->expr());
		}
		catch (InvalidSyntaxError) {
			throw InvalidSyntaxError("Expected ']' 'let', 'if', 'for', 'while', 'fn', int, float, identifier", current_token.GetStart().value(), current_token.GetEnd().value());
		}
		while (current_token.isToken(TypeToken::COMMA)) {
			advance();
			nodes.push_back(this->expr());
		}
		if (!current_token.isToken(TypeToken::RBRACKET)) {
			throw InvalidSyntaxError("Expected identifier or ']'", current_token.GetStart().value(), current_token.GetEnd().value());
		}
		advance();
	}


	return ListNode(nodes,start,current_token.GetEnd().value());
}


