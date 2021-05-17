#pragma once
#include <ostream>
#include <optional>
#include <string>
#include "TokenEnum.h"
#include "Position.h"

using namespace std;

class Token {
	public:
		Token();
		Token(TypeToken type, Position start, optional<Position> end = nullopt, optional<string> value = nullopt);
		optional<string> GetValue() const;
		Position GetStart() const { return start; };
		Position GetEnd() const { return end; };
		TypeToken GetType() const { return type; };
		bool matches(const TypeToken& token, string value);
		bool matchesKeyWord(string value);
		bool isToken(const TypeToken& token);
		friend std::ostream& operator<<(std::ostream& lhs, const Token& token);
	private:
		TypeToken type;
		optional<string> value = nullopt;
		Position start;
		Position end;
};

