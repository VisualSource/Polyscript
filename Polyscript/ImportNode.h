#pragma once
#include <string>
#include "Node.h"

class ImportNode : public Node {
	public:
		ImportNode(std::string path,Position start, Position end);
		std::string GetPath() const;
	private:
		std::string path;
};

