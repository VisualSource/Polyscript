#pragma once
#include <ostream>
#include <optional>
#include <string>
#include "TokenEnum.h"
#include "Position.h"

class Token {
	public:
		Token();
		Token(TypeToken type, Position start, std::optional<Position> end = std::nullopt, std::optional<std::string> value = std::nullopt);
		std::optional<std::string> GetValue() const;
		Position GetStart() const { return start; };
		Position GetEnd() const { return end; };
		TypeToken GetType() const { return type; };
		bool matches(const TypeToken& token, std::string value);
		bool matchesKeyWord(std::string value);
		bool isToken(const TypeToken& token);
		friend std::ostream& operator<<(std::ostream& lhs, const Token& token);
	private:
		TypeToken type;
		std::optional<std::string> value = std::nullopt;
		Position start;
		Position end;
};

