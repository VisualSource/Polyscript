#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Token.h"

using namespace std;

class PathAccessNode : public Node {
	public:
		PathAccessNode(const vector<Token>& path, Position start, Position end);
		vector<string> GetPath() const;
	private:
		vector<string> path;
};

