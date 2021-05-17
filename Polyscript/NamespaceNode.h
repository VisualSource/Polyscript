#pragma once
#include <any>
#include "Node.h"
#include "Token.h"

using namespace std;

class NamespaceNode : public Node {
	public:
		NamespaceNode(Token name, any statements, Position start, Position end);
		Token GetName() const;
		any GetStatements() const;
	private:
		Token name;
		any statements;
};

