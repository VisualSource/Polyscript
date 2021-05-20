#pragma once
#include <any>
#include <vector>
#include "Node.h"

class ListNode : public Node {
	public:
		ListNode(std::vector<std::any> elements, Position start, Position end);
		std::vector<std::any> GetElements() const;
	private:
		std::vector<std::any> elements;
};

