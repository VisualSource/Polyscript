#include "Token.h"

Token::Token() {
	value = nullopt;
	type = TypeToken::T_UNDEFINED;
}

Token::Token(TypeToken type, Position start, optional<Position> end, optional<string> value): type(type), value(value){

		this->start = start;

		if (end.has_value()) {
			this->end = end.value();
		}
		else {
			this->end = start;
			this->end.advance('\0');
		}
}

optional<string> Token::GetValue() const {
	return value;
}

bool Token::matches(const TypeToken& token, string value) {
	
	return token == type && value == this->value.value();
}

bool Token::matchesKeyWord(string value)
{
	return matches(TypeToken::KEYWORD,value);
}

bool Token::isToken(const TypeToken& token) {
	return type == token;
}

std::ostream& operator<<(std::ostream& lhs, const Token& token)
{
	lhs << TokenUtils::printTokenType(token.type);
	if (token.value.has_value()) {
		lhs << ":" << token.value.value();
	}
	return lhs;
}
