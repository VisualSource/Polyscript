#pragma once
#include <any>
#include "Node.h"
#include "Token.h"

class NamespaceNode : public Node {
	public:
		NamespaceNode(Token name, std::any statements, Position start, Position end);
		Token GetName() const;
		std::any GetStatements() const;
	private:
		Token name;
		std::any statements;
};

