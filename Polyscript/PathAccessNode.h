#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Token.h"

class PathAccessNode : public Node {
	public:
		PathAccessNode(const std::vector<Token>& path, Position start, Position end);
		std::vector<std::string> GetPath() const;
	private:
		std::vector<std::string> path;
};

