#pragma once
#include <string>
#include "Node.h"

class ImportNode : public Node {
	public:
		ImportNode(string path,Position start, Position end);
		string GetPath() const;
	private:
		string path;
};

