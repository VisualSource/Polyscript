#include "InterpreterUtils.h"
#include "Types.h"


void InterTypes::print(ostream& rhs, const any& node) {
	using namespace InterTypes;
	if (isTypeOf<Float>(node)) {
		rhs << any_cast<Float>(node);
	}
	else if (isTypeOf<Integer>(node)) {
		rhs << any_cast<Integer>(node);
	}
	else if (isTypeOf<Function>(node)) {
		rhs << any_cast<Function>(node);
	}
	else if (isTypeOf<String>(node)) {
		rhs << any_cast<String>(node);
	}
	else if (isTypeOf<List>(node)) {
		rhs << any_cast<List>(node);
	}
	else if (isTypeOf<BuiltInFunction>(node)) {
		rhs << any_cast<BuiltInFunction>(node);
	}
	else if (isTypeOf<Enum>(node)) {
		rhs << any_cast<Enum>(node);
	}
	else if (isTypeOf<EnumValue>(node)) {
		rhs << any_cast<EnumValue>(node);
	}
	else if (isTypeOf<Null>(node)) {
		rhs << any_cast<Null>(node);
	}
}

