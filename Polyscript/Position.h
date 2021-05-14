#pragma once
#include <string>

using namespace std;

class Position {
	public:
		Position(string fn = "<stdin>", string ftxt = "", int idx = -1, int ln = 0, int col = -1);
		Position& advance(char current_char);
		int GetIdx() const;
		int GetLn() const;
		int GetCol() const;
		string GetFn() const;
	private:
		int idx;
		int ln;
		int col;
		string fn;
		string ftxt;
};

