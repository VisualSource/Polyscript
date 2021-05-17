#pragma once
#include <string>
#include <vector>
#include "Variable.h"
#include "Token.h"
#include "Integer.h"

using namespace std;

class EnumValue;

class Enum : public Variable {
	public:
		Enum(const vector<Token>& keys, string name);
		virtual Enum& SetContext(Context* context);
		virtual Enum& SetPostion(Position start, Position end);
		EnumValue get(string key) const;
		bool empty() const;
		bool hasKey(string key) const;
		size_t len() const;
		friend ostream& operator<<(ostream& lhs, const Enum& rhs);
	private:
		vector<string> keys;
		string name = "anyomous";
};

