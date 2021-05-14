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
		Token(TypeToken type, optional<string> value = nullopt, optional<Position> start = nullopt, optional<Position> end = nullopt);
		optional<string> GetValue() const;
		optional<Position> GetStart() const { return start; };
		optional<Position> GetEnd() const { return end; };
		TypeToken GetType() const { return type; };
		bool matches(const TypeToken& token, string value);
		bool isToken(const TypeToken& token);
		friend std::ostream& operator<<(std::ostream& lhs, const Token& token);
	private:
		TypeToken type;
		optional<string> value = nullopt;
		optional<Position> start = nullopt;
		optional<Position> end = nullopt;
};

