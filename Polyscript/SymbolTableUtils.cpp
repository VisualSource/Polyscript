#include "SymbolTableUtils.h"
#include "RuntimeError.h"
#include "Context.h"

std::any ScopeTypes::VarToAny(const Var& value, Context* context)
{
	auto* typeA = std::get_if<Integer>(&value);
	if (typeA != nullptr) {
		return (*typeA);
	}

	auto* typeString = std::get_if<String>(&value);
	if (typeString != nullptr) {
		return (*typeString);
	}

	auto* typeB = std::get_if<Float>(&value);
	if (typeB != nullptr) {
		return (*typeB);
	}

	auto* func = std::get_if<Function>(&value);
	if (func != nullptr) {
		return *func;
	}

	auto* list = std::get_if<List>(&value);
	if (list != nullptr) {
		return (*list);
	}

	auto* builtin = std::get_if<BuiltInFunction>(&value);
	if (builtin != nullptr) {
		return *builtin;
	}

	auto* isEnum = std::get_if<Enum>(&value);
	if (isEnum != nullptr) {
		return *isEnum;
	}

	auto* isEnumValue = std::get_if<EnumValue>(&value);
	if (isEnumValue != nullptr) {
		return *isEnumValue;
	}

	auto* isNull = std::get_if<Null>(&value);
	if (isNull != nullptr) {
		return *isNull;
	}

	auto* isObject = std::get_if<Object>(&value);
	if (isObject != nullptr) {
		return *isObject;
	}

	throw RuntimeError("Failed to access variable", context, context->GetPostion(), context->GetPostion());
}
