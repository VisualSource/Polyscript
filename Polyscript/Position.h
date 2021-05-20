#pragma once
#include <string>

class Position {
	public:
		Position(std::string fn = "<stdin>", std::string ftxt = "", int idx = -1, int ln = 0, int col = -1);
		Position& advance(char current_char);
		int GetIdx() const;
		int GetLn() const;
		int GetCol() const;
		std::string GetFn() const;
	private:
		int idx;
		int ln;
		int col;
		std::string fn;
		std::string ftxt;
};

