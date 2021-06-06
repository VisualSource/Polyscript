#include "Position.h"

Position::Position(int idx, int ln, int col): idx(idx), ln(ln), col(col) {
}

Position& Position::advance(char current_char) {
	idx++;
	col++;
	if (current_char == '\n') {
		ln++;
		col = 0;
	}

	return *this;
}

int Position::GetIdx() const {
	return idx;
}

int Position::GetLn() const {
	return ln;
}

int Position::GetCol() const {
	return col;
}
