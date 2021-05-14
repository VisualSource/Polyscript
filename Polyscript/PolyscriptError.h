#pragma once
#include <string>
#include <ostream>
#include "Position.h"

using namespace std;

class PolyscriptError {
	public:
		PolyscriptError();
		PolyscriptError(string name, string details, Position start, Position end);
		Position GetStart() const { return start; };
		string as_str();
		friend std::ostream& operator<<(std::ostream& lhs, const PolyscriptError& token);
	private:
		Position start;
		Position end;
		string name;
		string details;
};

