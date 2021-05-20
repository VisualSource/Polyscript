#pragma once
#include <vector>
#include <any>
#include "Node.h"

struct MatchExpr {
	std::any checkValue;
	std::any expr;
	bool scoped;
};

struct MatchCatchAll {};

class MatchNode : public Node {
	public:
		MatchNode(std::any expr, const std::vector<MatchExpr>& matches, const Position& start, const Position& end);
		std::any GetExpr() const;
		std::vector<MatchExpr> GetMatches() const;
	private:
		std::vector<MatchExpr> matches;
		std::any expr;
};

