#include "Token.h"

Token::Token() {
	value = nullopt;
	type = TypeToken::T_UNDEFINED;
}

Token::Token(TypeToken type, optional<string> value, optional<Position> start, optional<Position> end): type(type), value(value){

	if (start.has_value()) {
		this->start = start;
		this->end = start;
		this->end.value().advance('\0');
	}
	if (end.has_value()) {
		this->end = end;
	}
}

optional<string> Token::GetValue() const {
	return value;
}

bool Token::matches(const TypeToken& token, string value) {
	
	return token == type && value == this->value.value();
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
