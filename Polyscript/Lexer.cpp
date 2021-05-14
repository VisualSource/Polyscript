#include "Lexer.h"
#include "TokenEnum.h"
#include <cctype>
#include "IllegalCharError.h"
#include "ExpectedCharError.h"

static bool isKeyWorld(const string& key) {
	if (key == "import") {
		return true;
	}
	// functions 
	if (key == "match" || key == "for" || key == "to" || key == "step" || key == "while" || key == "fn" || key == "if" || key == "else") return true;
	
	// Var/Types
	if (key == "let" || key == "int" || key == "float" || key == "bool") return true;
	
	return false;
}

Lexer::Lexer(string text, string fn): text(text) {
	current_char = '\0';
	pos = Position(fn, text);
	advance();
}

void Lexer::advance() {
	pos.advance(current_char);
	if (pos.GetIdx() < (int)text.length()) {
		current_char = text.at(pos.GetIdx());
	} else {
		current_char = '\0';
	}
}

vector<Token> Lexer::makeTokens() {
	vector<Token> tokens;

	while(current_char != '\0'){
		if(isblank(current_char)){
			advance();
		} else if(isdigit(current_char)){
			tokens.push_back(makeNumber());
		} else if (iscntrl(current_char)) {
			advance();
		} else if (isalpha(current_char) || current_char == '_') {
			tokens.push_back(makeIndentifier());
		}
		else if (current_char == ':') {
			tokens.push_back(Token(TypeToken::CONDITIONAL, nullopt, pos));
			advance();
		} else if (current_char == '^') {
			tokens.push_back(Token(TypeToken::POWER,nullopt,pos));
			advance();
		}else if (current_char == '+') {
			tokens.push_back(Token(TypeToken::PLUS, nullopt,pos));
			advance();
		}else if (current_char == '^') {
			tokens.push_back(Token(TypeToken::POWER,nullopt,pos));
			advance();
		}else if (current_char == '+') {
			tokens.push_back(Token(TypeToken::PLUS, nullopt,pos));
			advance();
		} else if (current_char == '-') {
			tokens.push_back(Token(TypeToken::MINUS, nullopt,pos));
			advance();
		} else if (current_char == '*') {
			tokens.push_back(Token(TypeToken::MUL, nullopt, pos));
			advance();
		} else if (current_char == '/') {
			tokens.push_back(Token(TypeToken::DIV,nullopt,pos));
			advance();
		} else if (current_char == '(') {
			tokens.push_back(Token(TypeToken::LPAREN,nullopt,pos));
			advance();
		}
		else if (current_char == ')') {
			tokens.push_back(Token(TypeToken::RPAREN, nullopt, pos));
			advance();
		}
		else if (current_char == '{') {
			tokens.push_back(Token(TypeToken::SCOPESTART, nullopt, pos));
			advance();
		}
		else if (current_char == '}') {
			tokens.push_back(Token(TypeToken::SCOPEEND, nullopt, pos));
			advance();
		}
		else if(current_char == ';'){
			tokens.push_back(Token(TypeToken::ENDSTATEMENT, nullopt, pos));
			advance();
		}
		else if (current_char == '|') {
			tokens.push_back(makeLogicOR());
		}else if(current_char == '&'){
			tokens.push_back(makeLogicAND());
		}
		else if (current_char == '!') {
			tokens.push_back(makeNotEquals());
		}
		else if (current_char == '=') {
			tokens.push_back(makeEquals());
		}
		else if (current_char == '<') {
			tokens.push_back(makeLessThan());
		}
		else if (current_char == '>') {
			tokens.push_back(makeGreaterThan());
		}
		else {
			Position start(pos);
			string error_char(1, current_char);
			advance();
			throw IllegalCharError(error_char,start,pos);
		}
	} 

	tokens.push_back(Token(TypeToken::T_EOF, nullopt,pos));
	return tokens;
}

Token Lexer::makeNumber() {
	Position start(pos);
	string num_str;
	int dot_count = 0;

	while (current_char != '\0' && (isdigit(current_char) || current_char == '.')) {
		 if (current_char == '.') {
			 if (dot_count == 1) break;
			 dot_count++;
			 num_str.push_back(current_char);
		 }else {
			 num_str.push_back(current_char);
		 }
		 advance();
	}

	if(dot_count == 0){
		return Token(TypeToken::INT, num_str, start, pos);
	} else {
		return Token(TypeToken::FLOAT, num_str, start, pos);
	}
}

Token Lexer::makeIndentifier() {
	string id_str;
	Position start(pos);

	while (current_char != '\0' && (isalnum(current_char) || current_char == '_')) {
		id_str.push_back(current_char);
		advance();
	}

	return Token(isKeyWorld(id_str) ? TypeToken::KEYWORD : TypeToken::IDENTIFIER, id_str,pos,start);
}

Token Lexer::makeEquals()
{
	TypeToken type = TypeToken::EQ;
	Position start(pos);
	advance();

	if (current_char == '=') {
		advance();
		type = TypeToken::EE;
	}

	return Token(type,nullopt,start,pos);
}

Token Lexer::makeNotEquals()
{
	TypeToken type = TypeToken::NOT;
	Position start(pos);
	advance();

	if (current_char == '=') {
		advance();
		type = TypeToken::NE;
	}

	return Token(type, nullopt, start, pos);
}

Token Lexer::makeLessThan()
{
	TypeToken type = TypeToken::LT;
	Position start(pos);
	advance();

	if (current_char == '=') {
		advance();
		type = TypeToken::LTE;
	}

	return Token(type, nullopt, start, pos);
}

Token Lexer::makeGreaterThan()
{
	TypeToken type = TypeToken::GT;
	Position start(pos);
	advance();

	if (current_char == '=') {
		advance();
		type = TypeToken::GTE;
	}

	return Token(type, nullopt, start, pos);
}

Token Lexer::makeLogicOR()
{
	Position start(pos);
	advance();

	if (current_char == '|') {
		advance();
		return Token(TypeToken::OR, nullopt, start, pos);
	}
	advance();
	throw ExpectedCharError("'|' after '|'", start, pos);
}

Token Lexer::makeLogicAND()
{
	Position start(pos);
	advance();

	if (current_char == '&') {
		advance();
		return Token(TypeToken::AND, nullopt, start, pos);
	}
	advance();
	throw ExpectedCharError("'&' after '&'", start, pos);
}
