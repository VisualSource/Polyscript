#pragma once
#include <string>

class Position {
	public:
		Position(int idx = -1, int ln = 0, int col = -1);
		Position& advance(char current_char);
		int GetIdx() const;
		int GetLn() const;
		int GetCol() const;
	private:
		int idx;
		int ln;
		int col;
};

