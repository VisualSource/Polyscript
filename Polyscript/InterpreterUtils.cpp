#include "InterpreterUtils.h"
#include "Types.h"

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

bool InterTypes::isEnum(const any& node)
{
	return node.type() == typeid(Enum);
}


void InterTypes::print(ostream& rhs, const any& node) {
	using namespace InterTypes;
	if (isFloat(node)) {
		rhs << any_cast<Float>(node);
	}
	else if (isInteger(node)) {
		rhs << any_cast<Integer>(node);
	}
	else if (isFunction(node)) {
		rhs << any_cast<Function>(node);
	}
	else if (isString(node)) {
		rhs << any_cast<String>(node);
	}
	else if (isList(node)) {
		rhs << any_cast<List>(node);
	}
	else if (isBuiltin(node)) {
		rhs << any_cast<BuiltInFunction>(node);
	}
	else if (isEnum(node)) {
		rhs << any_cast<Enum>(node);
	}
}

