#include "TokenEnum.h"

string TokenUtils::printTokenType(TypeToken token) {
	switch (token) {
		case TypeToken::DIV:
			return "DIV";
		case TypeToken::MUL:
			return "MUL";
		case TypeToken::PLUS:
			return "PLUS";
		case TypeToken::MINUS:
			return "MINUS";
		case TypeToken::INT:
			return "INT";
		case TypeToken::FLOAT:
			return "FLOAT";
		case TypeToken::LPAREN:
			return "LPAREM";
		case TypeToken::RPAREN:
			return "RPAREM";
		case TypeToken::T_EOF:
			return "EOF";
		case TypeToken::POWER:
			return "POWER";
		case TypeToken::EQ:
			return "EQ";
		case TypeToken::IDENTIFIER:
			return "IDENTIFIER";
		case TypeToken::KEYWORD:
			return "KEYWORD";
		case TypeToken::CONDITIONAL:
			return "CONDITIONAL";
		case TypeToken::NOT:
			return "NOT";
		case TypeToken::AND:
			return "AND";
		case TypeToken::OR:
			return "OR";
		case TypeToken::EE:
			return "EE";
		case TypeToken::NE:
			return "NE";
		case TypeToken::LT:
			return "LT";
		case TypeToken::GT:
			return "GT";
		case TypeToken::LTE:
			return "LTE";
		case TypeToken::GTE:
			return "GTE";
		case TypeToken::SCOPESTART:
			return "SCOPESTART";
		case TypeToken::SCOPEEND:
			return "SCOPEEND";
		default:
			return "[TOKEN NOT DEFINED]";
	}
}