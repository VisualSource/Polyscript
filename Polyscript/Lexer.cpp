#include "Lexer.h"
#include <cctype>
#include "TokenEnum.h"
#include "IllegalCharError.h"
#include "ExpectedCharError.h"


using namespace std;

static bool isKeyWorld(const string& key) {
	if (key == "import" || key == "throw" || key == "return" || key == "break" || key == "continue" || key == "namespace" || key == "class") {
		return true;
	}
	// functions 
	if (key == "match" || key == "for" || key == "to" || key == "step" || key == "while" || key == "fn" || key == "if" || key == "else") return true;
	
	// Var/Types
	if (key == "const" || key == "let" || key == "int" || key == "float" || key == "bool" || key == "string" || key == "enum") return true;
	
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
		}
		else if (current_char == '\n') {
			tokens.push_back(Token(TypeToken::NEWLINE, pos));
			advance();
		} else if (iscntrl(current_char)) {
			advance();
		} else if (isalpha(current_char) || current_char == '_') {
			tokens.push_back(makeIndentifier());
		}
		else if (current_char == ':') {
			TypeToken type = TypeToken::CONDITIONAL;
			advance();
			if (current_char == ':') {
				type = TypeToken::PATHSEP;
				advance();
			}
			tokens.push_back(Token(type, pos));
		} else if (current_char == '^') {
			tokens.push_back(Token(TypeToken::POWER,pos));
			advance();
		}else if (current_char == '+') {
			TypeToken type = TypeToken::PLUS;
			advance();

			if (current_char == '+') {
				type = TypeToken::PLUS_PLUS;
				advance();
			}
			else if (current_char == '=') {
				type = TypeToken::PLUS_EQ;
				advance();
			}

			tokens.push_back(Token(type,pos));
		}else if (current_char == '^') {
			tokens.push_back(Token(TypeToken::POWER,pos));
			advance();
		}else if (current_char == '+') {
			tokens.push_back(Token(TypeToken::PLUS, pos));
			advance();
		} else if (current_char == '-') {
			tokens.push_back(MakeMinusORRArrow());
		} else if (current_char == '*') {
			TypeToken type = TypeToken::MUL;
			advance();

			if (current_char == '=') {
				type = TypeToken::MUL_EQ;
				advance();
			}

			tokens.push_back(Token(type, pos));
		} else if (current_char == '/') {
			TypeToken type = TypeToken::DIV;
			advance();
			if (current_char == '/') {
				MakeComment();
				continue;
			}
			else if (current_char == '=') {
				type = TypeToken::DIV_EQ;
				advance();
			}

			tokens.push_back(Token(type, pos));
		} else if (current_char == '(') {
			tokens.push_back(Token(TypeToken::LPAREN, pos));
			advance();
		}
		else if (current_char == ',') {
			tokens.push_back(Token(TypeToken::COMMA, pos));
			advance();
		}
		else if (current_char == ')') {
			tokens.push_back(Token(TypeToken::RPAREN, pos));
			advance();
		}
		else if (current_char == '{') {
			tokens.push_back(Token(TypeToken::SCOPESTART, pos));
			advance();
		}
		else if (current_char == '}') {
			tokens.push_back(Token(TypeToken::SCOPEEND, pos));
			advance();
		}
		else if(current_char == ';'){
			tokens.push_back(Token(TypeToken::ENDSTATEMENT, pos));
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
		else if (current_char == '"') {
			tokens.push_back(makeString());
		}
		else if (current_char == '[') {
			tokens.push_back(Token(TypeToken::LBRACKET, pos));
			advance();
		}
		else if (current_char == ']') {
			tokens.push_back(Token(TypeToken::RBRACKET, pos));
			advance();
		}
		else if (current_char == '.') {
			tokens.push_back(Token(TypeToken::DOT, pos));
			advance();
		}
		else {
			Position start(pos);
			string error_char(1, current_char);
			advance();
			throw IllegalCharError(error_char,start,pos);
		}
	} 

	tokens.push_back(Token(TypeToken::T_EOF, pos));
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
		return Token(TypeToken::INT, start, pos, num_str);
	} else {
		return Token(TypeToken::FLOAT, start, pos, num_str);
	}
}

Token Lexer::makeIndentifier() {
	string id_str;
	Position start(pos);

	while (current_char != '\0' && (isalnum(current_char) || current_char == '_')) {
		id_str.push_back(current_char);
		advance();
	}

	return Token(isKeyWorld(id_str) ? TypeToken::KEYWORD : TypeToken::IDENTIFIER, start, pos, id_str);
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
	else if (current_char == '>') {
		advance();
		type = TypeToken::FATARROW;
	}

	return Token(type,start,pos);
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

	return Token(type, start, pos);
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

	return Token(type, start, pos);
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

	return Token(type, start, pos);
}

Token Lexer::makeLogicOR()
{
	Position start(pos);
	advance();

	if (current_char == '|') {
		advance();
		return Token(TypeToken::OR, start, pos);
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
		return Token(TypeToken::AND, start, pos);
	}
	advance();
	throw ExpectedCharError("'&' after '&'", start, pos);
}

Token Lexer::MakeMinusORRArrow()
{
	TypeToken type = TypeToken::MINUS;
	Position start(pos);
	advance();

	if (current_char == '>') {
		advance();
		type = TypeToken::FATARROW;
	}
	else if (current_char == '=') {
		type = TypeToken::MINUS_EQ;
		advance();
	}
	else if (current_char == '-') {
		type = TypeToken::MINUS_MINUS;
		advance();
	}

	return Token(type, start,pos);
}

Token Lexer::makeString()
{
	string value = "";
	Position start(pos);
	advance();

	while (current_char != '\0' && current_char != '"') {
		value.push_back(current_char);
		advance();
	}

	advance();
	return Token(TypeToken::STRING,start,pos, value);
}

void Lexer::MakeComment()
{
	advance();

	while (current_char != '\n')
	{
		advance();
	}

	advance();
}
