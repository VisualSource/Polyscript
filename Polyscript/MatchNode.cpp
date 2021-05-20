#include "MatchNode.h"

using namespace std;

MatchNode::MatchNode(any expr, const vector<MatchExpr>& matches, const Position& start, const Position& end): Node(start,end), expr(expr), matches(matches)
{
}

any MatchNode::GetExpr() const
{
	return expr;
}

vector<MatchExpr> MatchNode::GetMatches() const
{
	return matches;
}
