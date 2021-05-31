#include "Parser.h"
#include <optional>
#include "Nodes.h"
#include "NodeUtils.h"
#include "InvalidSyntaxError.h"
#include "TokenEnum.h"
#include "Token.h"

using namespace std;

Parser::Parser(vector<Token> tokens): tokens(tokens) {
	tok_idx = -1;
	advance();
}

any Parser::parse() {
	any res = statements();
	if (!current_token.isToken(TypeToken::T_EOF)) {
		throw InvalidSyntaxError("Expected ;", current_token.GetStart(), current_token.GetEnd());
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
	Position start(current_token.GetStart());

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

	return ListNode(statements,start,current_token.GetEnd());
}

any Parser::statement()
{
	Position start(current_token.GetStart());

	if (current_token.matches(TypeToken::KEYWORD, "return")) {
		advance();

		int reset = tok_idx;
		any statement = this->expr();
		if (!statement.has_value()) {
			tok_idx = reset;
			current_token = tokens.at(tok_idx);
		}
		return ReturnNode(statement, start, current_token.GetEnd());

	}
	if (current_token.matches(TypeToken::KEYWORD, "continue")) {
		advance();
		return ContinueNode(start, current_token.GetEnd());
	}
	if (current_token.matches(TypeToken::KEYWORD, "break")) {
		advance();
		return BreakNode(start, current_token.GetEnd());
	}

	return this->expr();
}

any Parser::atom(){
	Token tok = current_token;
	Position start(tok.GetStart());

	if (tok.isToken(TypeToken::INT) || tok.isToken(TypeToken::FLOAT)) {
		advance();
		return NumberNode(tok,start,tok.GetEnd());
	}
	else if (tok.isToken(TypeToken::STRING)) {
		advance();
		return StringNode(tok, start, tok.GetEnd());
	}
	else if (tok.isToken(TypeToken::IDENTIFIER)) {
		return Identifer();
	}
	else if (tok.isToken(TypeToken::SCOPESTART)) {
		advance();

		/*
		
		{
			IDENTIFER : expr ,
		}
		
		*/
		vector<ObjectNodeValue> objects;
		while (!current_token.isToken(TypeToken::SCOPEEND))
		{
			if (current_token.isToken(TypeToken::NEWLINE)) {
				advance();
				continue;
			}
			else if (current_token.isToken(TypeToken::IDENTIFIER)) {
				Token name = current_token;
				advance();
				if (!current_token.isToken(TypeToken::CONDITIONAL)) {
					throw InvalidSyntaxError("Expected ':'", current_token.GetStart(), current_token.GetEnd());
				}
				advance();

				any value = this->expr();

				if(current_token.isToken(TypeToken::COMMA)){
					advance();
				}

				objects.push_back(ObjectNodeValue {
						name,
						value
				});
			}
			else {
				throw InvalidSyntaxError("Expected a IDENTIFIER or ','", current_token.GetStart(), current_token.GetEnd());
			}
			
			
		}

		if(!current_token.isToken(TypeToken::SCOPEEND)){
			throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
		}

		advance();

		return ObjectNode(objects,tok.GetStart(),current_token.GetEnd());

	}
	else if (tok.isToken(TypeToken::LPAREN)) {
		advance();
		any expr = this->expr();
		if (current_token.isToken(TypeToken::RPAREN)) {
			advance();
			return expr;
		} else {
			throw InvalidSyntaxError("Expected ')'", current_token.GetStart(), current_token.GetEnd());
		}
	}
	else if (tok.isToken(TypeToken::LBRACKET)) {
		return listExpr();
	}
	else if (tok.matchesKeyWord("if")){
		return exprIf();
	}
	else if (tok.matchesKeyWord("for")) {
		return forExpr();
	}
	else if (tok.matchesKeyWord("while")) {
		return whileExpr();
	}
	else if(tok.matchesKeyWord("fn")){
		return funcDef();
	}
	else if (tok.matchesKeyWord("enum")) {
		return enumExpr();
	}
	else if (tok.matchesKeyWord("namespace")) {
		return namespaceDef();
	}
	else if (tok.matchesKeyWord("import")) {
		return importDef();
	}
	else if (tok.matchesKeyWord("match")) {
		return matchExpr();
	}

	return any();
}

/*
Function call;
atom (LPAREN (IDENTIFIER (COMMA IDENTIFIER)*)? RPAREN)?
*/
any Parser::call()
{
	Position start(current_token.GetStart());
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
				throw InvalidSyntaxError("Expected ')' 'let', 'if', 'for', 'while', 'fn', int, float, identifier" , current_token.GetStart(), current_token.GetEnd());
			}
			while (current_token.isToken(TypeToken::COMMA)) {
				advance();
				arg_nodes.push_back(this->expr());
			}
			if (!current_token.isToken(TypeToken::RPAREN)) {
				throw InvalidSyntaxError("Expected identifier or ')'", current_token.GetStart(), current_token.GetEnd());
			}
			advance();
		}
		return FuncCallNode(atom,arg_nodes, start,current_token.GetEnd());
	}

	
	return atom;
}
any Parser::power() {
	Position start(current_token.GetStart());
	any left = this->call();

	while (current_token.isToken(TypeToken::POWER)) {
		Token op_tok = current_token;
		advance();
		any right = factor();
		left = BinOpNode(left, op_tok, right, start, current_token.GetEnd());
	}
	return left;
}

any Parser::factor(){
	Token tok = current_token;

	if (tok.isToken(TypeToken::PLUS) || tok.isToken(TypeToken::MINUS)) {
		advance();
		any factor = this->factor();
		return UnaryOpNode(tok, factor,tok.GetStart(),current_token.GetEnd());
	} 

	return power();
}

any Parser::term(){
	Position start(current_token.GetStart());
	any left = factor();

	while (current_token.isToken(TypeToken::MUL) || current_token.isToken(TypeToken::DIV)) {
		Token op_tok = current_token;
		advance();
		any right = factor();
		left = BinOpNode(left,op_tok,right,start,current_token.GetEnd());
	}
	return left;
}


/*
 Creation of 

 KEYWORD:let IDENETIFER (CONDIONAL KEYWORD)? EQ expr
 AND|OR

*/
any Parser::expr() {
	Position start(current_token.GetStart());
	if (current_token.matchesKeyWord("let") || current_token.matchesKeyWord("const")) {
		bool isConstant = !current_token.matchesKeyWord("const");
		advance();

		if (!current_token.isToken(TypeToken::IDENTIFIER)) {
			throw InvalidSyntaxError("Expected identifier", current_token.GetStart(), current_token.GetEnd());
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
				throw InvalidSyntaxError("Expected Type identifier", current_token.GetStart(), current_token.GetEnd());
			}
		}

		if (!current_token.isToken(TypeToken::EQ)) {
			throw InvalidSyntaxError("Expected assigment", current_token.GetStart(), current_token.GetEnd());
		}
		advance();

		any expr = this->expr();
		
		return VarAssignNode(varName,isConstant,expr,start,current_token.GetEnd(),varType);
	}


	
	any left = compExpr();
	while (current_token.isToken(TypeToken::AND) || current_token.isToken(TypeToken::OR)) {
		Token op_tok = current_token;
		advance();
		any right = compExpr();
		left = BinOpNode(left, op_tok, right,start,current_token.GetEnd());
	}
	return left;
}

any Parser::compExpr()
{
	Position start(current_token.GetStart());
	if (current_token.isToken(TypeToken::NOT)) {
		Token op_tok = current_token;
		advance();
		any node = compExpr();
		return UnaryOpNode(op_tok,node,start,current_token.GetEnd());
	}

	any left = arithExpr();

	while (current_token.isToken(TypeToken::EE) || current_token.isToken(TypeToken::NE) || current_token.isToken(TypeToken::LT) 
		|| current_token.isToken(TypeToken::GT) || current_token.isToken(TypeToken::LTE) || current_token.isToken(TypeToken::GTE)) {
		Token op_tok = current_token;
		advance();
		any right = arithExpr();
		left = BinOpNode(left, op_tok, right,start,current_token.GetEnd());
	}

	return left;
}

any Parser::arithExpr()
{
	Position start(current_token.GetStart());
	any left = term();

	while (current_token.isToken(TypeToken::PLUS) || current_token.isToken(TypeToken::MINUS)){
		Token op_tok = current_token;
		advance();
		any right = term();
		left = BinOpNode(left, op_tok, right,start,current_token.GetEnd());
	}

	return left;
}

// Grammer KEYWORD:if expr { (expr*)? } (else (if {}) 
any Parser::exprIf()
{
	Position start(current_token.GetStart());
	vector<IfCases> cases;
	any else_case;

	if (!current_token.matches(TypeToken::KEYWORD, "if")) {
		throw InvalidSyntaxError("Expexted 'if'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	any condition = expr();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'",current_token.GetStart(),current_token.GetEnd());
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
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	while (current_token.matches(TypeToken::KEYWORD, "else")) {
		advance();
		if (current_token.matches(TypeToken::KEYWORD, "if")) {
			advance();
			any condition = expr();
			if (!current_token.isToken(TypeToken::SCOPESTART)) {
				throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
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
				throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
			}
			advance();
		}
		else if (current_token.isToken(TypeToken::SCOPESTART)) {
			advance();
			else_case = statements();

			if (!current_token.isToken(TypeToken::SCOPEEND)) {
				throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
			}
			advance();
		}
		else {
			throw InvalidSyntaxError("Expected 'if' or '{'",current_token.GetStart(),current_token.GetEnd());
		}
		
	}


	return IfNode(cases, else_case,start,current_token.GetEnd());
}

// Creation of a for loop block
// Grammer: KEYWORD:for IDENTIFIER EQ expr KEYWORD:to expr SCOPESTART expr SCOPEEND
any Parser::forExpr()
{
	Position start(current_token.GetStart());
	if (!current_token.matches(TypeToken::KEYWORD, "for")) {
		throw InvalidSyntaxError("Expected 'for'",current_token.GetStart(),current_token.GetEnd());
	}

	advance();

	if(!current_token.isToken(TypeToken::IDENTIFIER)){
		throw InvalidSyntaxError("Expected identifier", current_token.GetStart(), current_token.GetEnd());
	}

	Token varName = current_token;
	advance();

	if (!current_token.isToken(TypeToken::EQ)) {
		throw InvalidSyntaxError("Expected '='",current_token.GetStart(),current_token.GetEnd());
	}
	advance();

	any start_value = this->expr();

	if (!current_token.matches(TypeToken::KEYWORD, "to")) {
		throw InvalidSyntaxError("Expected 'to'",current_token.GetStart(),current_token.GetEnd());
	}

	advance();

	any end_value = this->expr();

	any step_value;
	if (current_token.matches(TypeToken::KEYWORD, "step")) {
		advance();

		step_value = this->expr();
	}

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	any body = this->statements();


	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();


	return ForNode(varName,start_value,end_value,body,step_value,start,current_token.GetStart());
}

/*
Grammer 

KEYWORLD:enum IDENTIFIER SCOPESTART (IDENTIFER (COMMA)?)? SCOPEEND

*/
any Parser::enumExpr()
{
	Position start(current_token.GetStart());
	if (!current_token.matchesKeyWord("enum")) {
		throw InvalidSyntaxError("Expected keyworld 'enum'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	if (!current_token.isToken(TypeToken::IDENTIFIER)) {
		throw InvalidSyntaxError("Expected Identifier", current_token.GetStart(), current_token.GetEnd());
	}
	string name = current_token.GetValue().value();
	advance();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected Identifier", current_token.GetStart(), current_token.GetEnd());
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
			throw InvalidSyntaxError("Expected Identifier or ','", current_token.GetStart(), current_token.GetEnd());
		}
	}

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	return EnumNode(name,enumsKeys,start,current_token.GetEnd());
}

// Grammer: KEYWORD:while expr SCOPESTART expr SCOPEEND
any Parser::whileExpr()
{
	Position start(current_token.GetStart());
	if(!current_token.matches(TypeToken::KEYWORD,"while")){
		throw InvalidSyntaxError("Expected 'while'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	any condition = this->expr();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	any body = this->statements();

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	return WhileNode(condition,body,start,current_token.GetEnd());
}

/*
	Gammer: KEYWORD:fn IDENTIFIER? LPAREN (IDENTIFIER (COMMA IDENTIFIER)*)? RPAREN (RARROW TYPE) ? SCOPESTART expr SCOPEEND
*/
any Parser::funcDef()
{
	Position start(current_token.GetStart());
	if (!current_token.matches(TypeToken::KEYWORD, "fn")) {
		throw InvalidSyntaxError("Expected 'fn'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();
	optional<Token> var_name = nullopt;
	if (current_token.isToken(TypeToken::IDENTIFIER)) {
		var_name = current_token;
		advance();
		if (!current_token.isToken(TypeToken::LPAREN)) {
			throw InvalidSyntaxError("Expected '('", current_token.GetStart(), current_token.GetEnd());
		}
	}
	else {
		if (!current_token.isToken(TypeToken::LPAREN)) {
			throw InvalidSyntaxError("Expected identifier or '('", current_token.GetStart(), current_token.GetEnd());
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
				throw InvalidSyntaxError("Expected identifier", current_token.GetStart(), current_token.GetEnd());
			}
			arg_names.push_back(current_token);
			advance();
		}
		if (!current_token.isToken(TypeToken::RPAREN)) {
			throw InvalidSyntaxError("Expected ',' or ')'", current_token.GetStart(), current_token.GetEnd());
		}
	}
	else {
		if (!current_token.isToken(TypeToken::RPAREN)) {
			throw InvalidSyntaxError("Expected identifier or ')'", current_token.GetStart(), current_token.GetEnd());
		}
	}

	advance();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	any return_node = this->statements();

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	return FuncDefNode(var_name,arg_names,return_node,start,current_token.GetEnd());
}

any Parser::listExpr()
{
	vector<any> nodes;
	Position start(current_token.GetStart());

	if (!current_token.isToken(TypeToken::LBRACKET)) {
		throw InvalidSyntaxError("Expected '['", current_token.GetStart(), current_token.GetEnd());
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
			throw InvalidSyntaxError("Expected ']' 'let', 'if', 'for', 'while', 'fn', int, float, identifier", current_token.GetStart(), current_token.GetEnd());
		}
		while (current_token.isToken(TypeToken::COMMA)) {
			advance();
			nodes.push_back(this->expr());
		}
		if (!current_token.isToken(TypeToken::RBRACKET)) {
			throw InvalidSyntaxError("Expected identifier or ']'", current_token.GetStart(), current_token.GetEnd());
		}
		advance();
	}


	return ListNode(nodes,start,current_token.GetEnd());
}

/*
Gammer: KEYWORD:namespace IDENTIFER SCOPESTART (statements) SCOPEEND ENDSTATMENT

*/
any Parser::namespaceDef()
{
	Position start(current_token.GetStart());
	if (!current_token.matchesKeyWord("namespace")) {
		throw InvalidSyntaxError("Expected keyworld 'namespace'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	if (!current_token.isToken(TypeToken::IDENTIFIER)) {
		throw InvalidSyntaxError("Expected identifier", current_token.GetStart(), current_token.GetEnd());
	}
	Token name = current_token;
	
	advance();

	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	any body = this->statements();

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();



	return NamespaceNode(name,body,start,current_token.GetEnd());
}

any Parser::importDef()
{
	Position start(current_token.GetStart());
	if (!current_token.matchesKeyWord("import")) {
		throw InvalidSyntaxError("Expected keyworld 'import'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	if (!current_token.isToken(TypeToken::STRING)) {
		throw InvalidSyntaxError("Expected string", current_token.GetStart(), current_token.GetEnd());
	}
	string path = current_token.GetValue().value();

	advance();

	if (!current_token.isToken(TypeToken::ENDSTATEMENT)) {
		throw InvalidSyntaxError("Expected ';'", current_token.GetStart(), current_token.GetEnd());
	}

	advance();

	return ImportNode(path, start, current_token.GetEnd());

}

/*

KEYWORLD:match expr SCOPESTART ((EnumValue|Integer|String|Float|IDENTIFIER:_) FATARROW (expr|SCOPESTART statments SCOPEEND) (COMMA)?)* SCOPEEND;



*/
any Parser::matchExpr()
{
	Position start(current_token.GetStart());
	if (!current_token.matchesKeyWord("match")) {
		throw InvalidSyntaxError("Expected keyworld 'match'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	any expr = this->expr();

	
	if (!current_token.isToken(TypeToken::SCOPESTART)) {
		throw InvalidSyntaxError("Expected '{'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	vector<MatchExpr> matches;

	while (!current_token.isToken(TypeToken::SCOPEEND))
	{
		if (current_token.isToken(TypeToken::NEWLINE)) {
			advance();
			continue;
		}
		matches.push_back(makeMatchExpr());

		
		if (current_token.isToken(TypeToken::COMMA)) {
			advance();
		}
			

	}

	while (current_token.isToken(TypeToken::NEWLINE)) {
		advance();
	}

	if (!current_token.isToken(TypeToken::SCOPEEND)) {
		throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();


	return MatchNode(expr, matches, start, current_token.GetEnd());

}
/*

 IDENTIFER | (IDENTIFER LBRACKET expr RBRACKET) | (IDENTIFER PATHSEP IDENTIFER (PATHSEP IDENTIFER)* ) 

 IDENTIFER (PLUS_EQ|MINUS_EQ|DIV_EQ|MUL_EQ|EQ) expr
 
 
 */
any Parser::Identifer(bool checkEQ)
{
	Token tok = current_token;
	Position start(tok.GetStart());
	advance();

	if (checkEQ && current_token.isToken(TypeToken::LBRACKET)) {
		advance();

		any index = this->expr();

		if (!current_token.isToken(TypeToken::RBRACKET)) {
			throw InvalidSyntaxError("Expected ']'", current_token.GetStart(), current_token.GetEnd());
		}
		advance();

		return ListAccessNode(tok, index, start, current_token.GetEnd());

	}
	else if (current_token.isToken(TypeToken::PATHSEP)) {
		vector<Token> path;
		path.push_back(tok);

		while (current_token.isToken(TypeToken::PATHSEP))
		{
			advance();
			if (!current_token.isToken(TypeToken::IDENTIFIER)) {
				throw InvalidSyntaxError("Expected IDENTIFIER", current_token.GetStart(), current_token.GetEnd());
			}
			path.push_back(current_token);
			advance();
		}

		//advance();

		return PathAccessNode(path, start, current_token.GetEnd());
	}
	else if (checkEQ && (current_token.isToken(TypeToken::PLUS_EQ) || current_token.isToken(TypeToken::MINUS_EQ) || current_token.isToken(TypeToken::MUL_EQ) || current_token.isToken(TypeToken::DIV_EQ) || current_token.isToken(TypeToken::EQ))) {
		Token op = current_token;
		advance();

		any value = this->expr();

		return VarReasignNode(tok, op, value, start, current_token.GetEnd());

	}
	else if (current_token.isToken(TypeToken::DOT)) {
		vector<string> path;
		path.push_back(tok.GetValue().value());

		while (current_token.isToken(TypeToken::DOT))
		{
			advance();
			if (!current_token.isToken(TypeToken::IDENTIFIER)) {
				throw InvalidSyntaxError("Expected IDENTIFIER", current_token.GetStart(), current_token.GetEnd());
			}
			path.push_back(current_token.GetValue().value());
			advance();
		}
		
		return DotAccessNode(path,tok.GetStart(),current_token.GetEnd());
	}

	return VarAccessNode(tok, start, tok.GetEnd());
}

/*

	(EnumValue|Integer|String|Float|IDENTIFIER:_) FATARROW (expr|SCOPESTART statments SCOPEEND) (COMMA)?

*/
MatchExpr Parser::makeMatchExpr()
{
	Position start(current_token.GetStart());
	any checkValue;
	if (current_token.isToken(TypeToken::INT) || current_token.isToken(TypeToken::FLOAT)) {
		checkValue = NumberNode(current_token, start, current_token.GetEnd());
		advance();
	}
	else if (current_token.isToken(TypeToken::STRING)) {
		checkValue = StringNode(current_token, start, current_token.GetEnd());
		advance();
	}
	else if (current_token.isToken(TypeToken::IDENTIFIER)) {
		if (current_token.matches(TypeToken::IDENTIFIER, "_")) {
			checkValue = MatchCatchAll {};
			advance();
		}
		else {
			checkValue = Identifer(false);
		}
	}
	else {
		throw InvalidSyntaxError("Expected INT|FLOAT|STRING|ENUM or '_' ", current_token.GetStart(), current_token.GetEnd());
	}

	//advance();

	if (!current_token.isToken(TypeToken::FATARROW)) {
		throw InvalidSyntaxError("Expected '=>'", current_token.GetStart(), current_token.GetEnd());
	}
	advance();

	any body;
	bool scoped = false;
	if (current_token.isToken(TypeToken::SCOPESTART)) {
		advance();
		scoped = true;
		body = this->statements();
		if (!current_token.isToken(TypeToken::SCOPEEND)) {
			throw InvalidSyntaxError("Expected '}'", current_token.GetStart(), current_token.GetEnd());
		}
		advance();
	}else{
		body = this->statement();
	}

	return MatchExpr{ checkValue, body, scoped };
}


