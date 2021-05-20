#pragma once
#include <string>
#include <vector>
#include "Variable.h"
#include "Token.h"
#include "Integer.h"

class EnumValue;

class Enum : public Variable {
	public:
		Enum(const std::vector<Token>& keys, std::string name);
		virtual Enum& SetContext(Context* context);
		virtual Enum& SetPostion(Position start, Position end);
		EnumValue get(std::string key) const;
		bool empty() const;
		bool hasKey(std::string key) const;
		size_t len() const;
		friend std::ostream& operator<<(std::ostream& lhs, const Enum& rhs);
	private:
		std::vector<std::string> keys;
		std::string name = "anyomous";
};

