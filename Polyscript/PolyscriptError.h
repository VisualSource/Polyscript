#pragma once
#include <string>
#include <ostream>
#include "Position.h"

class PolyscriptError {
	public:
		PolyscriptError();
		PolyscriptError(std::string name, std::string details, Position start, Position end);
		Position GetStart() const { return start; };
		std::string as_str(std::string ad = "");
		friend std::ostream& operator<<(std::ostream& lhs, const PolyscriptError& token);
	private:
		Position start;
		Position end;
		std::string name;
		std::string details;
};

