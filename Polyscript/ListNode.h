#pragma once
#include <any>
#include <vector>
#include "Node.h"

using namespace std;

class ListNode : public Node {
	public:
		ListNode(vector<any> elements, Position start, Position end);
		vector<any> GetElements() const;
	private:
		vector<any> elements;
};

