#pragma once
#include <string>
#include <vector>
#include "Node.h"
class DotAccessNode : public Node {
	public:
		DotAccessNode(const std::vector<std::string>& accessPath, Position start, Position end);
		std::vector<std::string> GetAccessPath() const;
	private:
		std::vector<std::string> accessPath;
};

