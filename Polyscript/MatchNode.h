#pragma once
#include <vector>
#include <any>
#include "Node.h"

using namespace std;

struct MatchExpr {
	any checkValue;
	any expr;
	bool scoped;
};

struct MatchCatchAll {};

class MatchNode : public Node {
	public:
		MatchNode(any expr, const vector<MatchExpr>& matches, const Position& start, const Position& end);
		any GetExpr() const;
		vector<MatchExpr> GetMatches() const;
	private:
		vector<MatchExpr> matches;
		any expr;
};

