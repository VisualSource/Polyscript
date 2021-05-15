#include "InterpreterUtils.h"
#include "Integer.h"
#include "Float.h"
#include "Function.h"
#include "String.h"
#include "List.h"
#include "BuiltInFunction.h"

bool InterTypes::isFloat(const any& node) {
	return node.type() == typeid(Float);
}
bool InterTypes::isInteger(const any& node) {
	return node.type() == typeid(Integer);
}

bool InterTypes::isFunction(const any& node)
{
	return node.type() == typeid(Function);
}

bool InterTypes::isString(const any& node)
{
	return node.type() == typeid(String);
}

bool InterTypes::isList(const any& node)
{
	return node.type() == typeid(List);
}

bool InterTypes::isBuiltin(const any& node)
{
	return node.type() == typeid(BuiltInFunction);
}


void InterTypes::print(ostream& rhs, const any& node) {
	if (InterTypes::isFloat(node)) {
		rhs << any_cast<Float>(node);
	}
	else if (InterTypes::isInteger(node)) {
		rhs << any_cast<Integer>(node);
	}
	else if (InterTypes::isFunction(node)) {
		rhs << any_cast<Function>(node);
	}
	else if (InterTypes::isString(node)) {
		rhs << any_cast<String>(node);
	}
	else if (InterTypes::isList(node)) {
		rhs << any_cast<List>(node);
	}
	else if (InterTypes::isBuiltin(node)) {
		rhs << any_cast<BuiltInFunction>(node);
	}
}

