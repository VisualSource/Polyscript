#include "Interpreter.h"
#include <string>
#include <fstream>
#include <variant>
#include "InterpreterUtils.h"
#include "SymbolTable.h"
#include "PolyscriptError.h"
#include "RuntimeError.h"
#include "Nodes.h"
#include "Lexer.h"
#include "Parser.h"
#include "fetch.h"


using namespace std;

Interpreter::Interpreter() {
}


any Interpreter::visit(const any& node, Context* context) {
	using namespace NodeUtils;
	if (isNode<BinOpNode>(node)) {
		return visit_BinOpNode(any_cast<BinOpNode>(node), context);
	}
	else if (isNode<UnaryOpNode>(node)) {
		return visit_UnaryOpNode(any_cast<UnaryOpNode>(node), context);
	}
	else if (isNode<NumberNode>(node)) {
		return visit_NumberNode(any_cast<NumberNode>(node), context);
	}
	else if (isNode<VarAssignNode>(node)) {
		return visit_VarAssignNode(any_cast<VarAssignNode>(node), context);
	}
	else if (isNode<VarAccessNode>(node)) {
		return visit_VarAccessNode(any_cast<VarAccessNode>(node), context);
	}
	else if (isNode<IfNode>(node)) {
		return visit_IfNode(any_cast<IfNode>(node), context);
	}
	else if (isNode<ForNode>(node)) {
		return visit_ForNode(any_cast<ForNode>(node), context);
	}
	else if (isNode<WhileNode>(node)) {
		return visit_WhileNode(any_cast<WhileNode>(node), context);
	}
	else if (isNode<FuncDefNode>(node)) {
		return visit_FuncDefNode(any_cast<FuncDefNode>(node), context);
	}else if(isNode<FuncCallNode>(node)){
		return visit_FuncCallNode(any_cast<FuncCallNode>(node), context);
	} else if(isNode<StringNode>(node)){
		return visit_StringNode(any_cast<StringNode>(node), context);
	}
	else if (isNode<ListNode>(node)) {
		return visit_ListNode(any_cast<ListNode>(node), context);
	}
	else if (isNode<ListAccessNode>(node)) {
		return visit_ListAccessNode(any_cast<ListAccessNode>(node), context);
	}
	else if (isNode<ReturnNode>(node)) {
		return visit_ReturnNode(any_cast<ReturnNode>(node),context);
	}
	else if (isNode<BreakNode>(node)) {
		throw BreakEvent();
	}
	else if (isNode<ContinueNode>(node)) {
		throw ContinueEvent();
	}
	else if (isNode<EnumNode>(node)) {
		return visit_EnumDefNode(any_cast<EnumNode>(node),context);
	}
	else if (isNode<PathAccessNode>(node)) {
		return visit_PathAccessNode(any_cast<PathAccessNode>(node), context);
	}
	else if (isNode<VarReasignNode>(node)) {
		return visit_VarReasignNode(any_cast<VarReasignNode>(node), context);
	}
	else if (isNode<NamespaceNode>(node)) {
		return visit_NamespaceNode(any_cast<NamespaceNode>(node), context);
	}
	else if (isNode<ImportNode>(node)) {
		return visit_ImportNode(any_cast<ImportNode>(node), context);
	}
	else if (isNode<MatchNode>(node)) {
		return visit_MatchNode(any_cast<MatchNode>(node), context);
	}
	else if (isNode<ObjectNode>(node)) {
		return visit_ObjectNode(any_cast<ObjectNode>(node), context);
	}
	else if (isNode<DotAccessNode>(node)) {
		return visit_DotAccessNode(any_cast<DotAccessNode>(node),context);
	}
	else {
		throw PolyscriptError("Undefined Operation", "No Visit Method for: " + string(node.type().name()), Position(), Position());
	}

}

any Interpreter::visit_NumberNode(const NumberNode& node, Context* context) {
	Token n = node.GetToken();
	if (n.isToken(TypeToken::INT)) {
		Integer number(std::stoi(n.GetValue().value()));
		number.SetContext(context);
		number.SetPostion(n.GetStart(),n.GetEnd());
		return number;
	} else if (n.isToken(TypeToken::FLOAT)) {
		Float number(std::stod(n.GetValue().value()));
		number.SetPostion(n.GetStart(), n.GetEnd());
		number.SetContext(context);
		return number;
	}
	else {
		throw PolyscriptError("Invaild number","Can't parse number",node.GetStart(),node.GetEnd());
	}

}

any Interpreter::visit_BinOpNode(const BinOpNode& node, Context* context) {
	using namespace InterTypes;
	any left = visit(node.GetLeftNode(),context);
	any right = visit(node.GetRightNode(),context);

	switch (node.GetOpToken().GetType()) {
		case TypeToken::PLUS: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat + rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt + rightInt;
			}
			else if (isTypeOf<String>(left) && isTypeOf<String>(right)) {
				String leftInt = any_cast<String>(left);
				String rightInt = any_cast<String>(right);
				return leftInt + rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(),node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::MINUS: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat - rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt - rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::MUL: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat * rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt * rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::DIV: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat / rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt / rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::POWER: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat.power(rightFloat);
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt.power(rightInt);
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::EE: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				return any_cast<Float>(left) == any_cast<Float>(right);
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				return any_cast<Integer>(left) == any_cast<Integer>(right);
			}
			else if (isTypeOf<String>(left) && isTypeOf<String>(right)) {
				return any_cast<String>(left) == any_cast<String>(right);
			}
			else if (isTypeOf<EnumValue>(left) && isTypeOf<EnumValue>(right)) {
				return any_cast<EnumValue>(left) == any_cast<EnumValue>(right);
			}
			else if (isTypeOf<Object>(left) && isTypeOf<Object>(right)) {
				return Integer::False().SetContext(context);
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::NE: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				return any_cast<Float>(left) != any_cast<Float>(right);
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				return any_cast<Integer>(left) != any_cast<Integer>(right);
			}
			else if (isTypeOf<String>(left) && isTypeOf<String>(right)) {
				return any_cast<String>(left) != any_cast<String>(right);
			}
			else if (isTypeOf<EnumValue>(left) && isTypeOf<EnumValue>(right)) {
				return any_cast<EnumValue>(left) != any_cast<EnumValue>(right);
			}
			else if (isTypeOf<Object>(left) && isTypeOf<Object>(right)) {
				return Integer::True().SetContext(context);
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::LT: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat < rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt < rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::GT: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat > rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt > rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::LTE: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat >= rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt >= rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::GTE: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat <= rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt <= rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::AND: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Integer leftFloat = Integer::cast(any_cast<Float>(left));
				Integer rightFloat = Integer::cast(any_cast<Float>(right));
				return leftFloat && rightFloat;
			}else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt && rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		case TypeToken::OR: {
			if (isTypeOf<Float>(left) && isTypeOf<Float>(right)) {
				Integer leftFloat = Integer::cast(any_cast<Float>(left));
				Integer rightFloat = Integer::cast(any_cast<Float>(right));
				return leftFloat || rightFloat;
			}
			else if (isTypeOf<Integer>(left) && isTypeOf<Integer>(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt || rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
			}
		}
		default:
			throw RuntimeError("Undefined Operation", context, node.GetOpToken().GetStart(), node.GetOpToken().GetEnd());
	}
}

any Interpreter::visit_UnaryOpNode(const UnaryOpNode& node, Context* context) {
	using InterTypes::isTypeOf;
	any number = visit(node.GetNode(),context);

	if (node.GetToken().isToken(TypeToken::MINUS)) {
		if (isTypeOf<Float>(number)) {
			Float nodeF = any_cast<Float>(number);
			return nodeF * Float(-1.0);
		} else if (isTypeOf<Integer>(number)) {
			Integer nodeI = any_cast<Integer>(number);
			return nodeI * Integer(-1);
		}else {
			string className = number.type().name();
			throw RuntimeError("Invailed number type: " + className, context, node.GetToken().GetStart(), node.GetToken().GetEnd());
		}
	}
	else if (node.GetToken().isToken(TypeToken::NOT)) {
		if (isTypeOf<Float>(number)) {
			Float nodeF = any_cast<Float>(number);
			return !Integer::cast(nodeF);
		}
		else if (isTypeOf<Integer>(number)) {
			Integer nodeI = any_cast<Integer>(number);
			return !nodeI;
		}
		else {
			string className = number.type().name();
			throw RuntimeError("Invailed type: " + className, context, node.GetToken().GetStart(), node.GetToken().GetEnd());
		}
	}
	return number;
}

any Interpreter::visit_VarAccessNode(const VarAccessNode& node, Context* context)
{
	string var_name = node.GetToken().GetValue().value();
	ScopeTypes::Var value = context->GetScope()->get(var_name,context);

	auto* typeA = std::get_if<Integer>(&value);
	if (typeA != nullptr) {
		return (*typeA).SetContext(context);
	}

	auto* typeString = get_if<String>(&value);
	if (typeString != nullptr) {
		return (*typeString).SetContext(context);
	}

	auto* typeB = std::get_if<Float>(&value);
	if (typeB != nullptr) {
		return (*typeB).SetContext(context);
	}

	auto* func = std::get_if<Function>(&value);
	if (func != nullptr) {
		return *func;
	}

	auto* list = get_if<List>(&value);
	if (list != nullptr) {
		return (*list).SetContext(context);
	}

	auto* builtin = get_if<BuiltInFunction>(&value);
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

	throw RuntimeError("Failed to access variable", context,node.GetStart(),node.GetEnd());
}

any Interpreter::visit_VarAssignNode(const VarAssignNode& node, Context* context)
{
	using InterTypes::isTypeOf;
	string var_name = node.GetToken().GetValue().value();

	any value = visit(node.GetNode(),context);

	
	if (isTypeOf<Integer>(value)) {
		if (node.GetVarType().matchesKeyWord("float")) {
			Float realF = Float::cast(any_cast<Integer>(value));
			context->GetScope()->add(var_name, realF,context);
			return realF;
		} else if(node.GetVarType().matchesKeyWord("string")){
			String real = String::cast(any_cast<Integer>(value));
			context->GetScope()->add(var_name, real,context);
			return real;
		}else {
			context->GetScope()->add(var_name, any_cast<Integer>(value),context);
		}
	}
	else if (isTypeOf<Float>(value)) {
			if (node.GetVarType().matchesKeyWord("int")) {
				Integer realF = Integer::cast(any_cast<Float>(value));
				context->GetScope()->add(var_name, realF,context);
				return realF;
			}
			else if (node.GetVarType().matchesKeyWord("string")) {
				String real = String::cast(any_cast<Float>(value));
				context->GetScope()->add(var_name, real,context);
				return real;
			}
			else {
				context->GetScope()->add(var_name, any_cast<Float>(value),context);
			}
	}
	else if (isTypeOf<String>(value)) {
		if (node.GetVarType().matchesKeyWord("int")) {
			Integer real = Integer::cast(any_cast<String>(value));
			context->GetScope()->add(var_name, real,context);
			return real;
		}
		else if (node.GetVarType().matchesKeyWord("float")) {
			Float real = Float::cast(any_cast<String>(value));
			context->GetScope()->add(var_name, real,context);
			return real;
		}
		else {
			context->GetScope()->add(var_name, any_cast<String>(value),context);
		}
	}
	else if (isTypeOf<List>(value)) {
		context->GetScope()->add(var_name, any_cast<List>(value),context);
	}
	else if (isTypeOf<EnumValue>(value)) {
		if (node.GetVarType().matchesKeyWord("int")) {
			Integer real = EnumValue::cast(any_cast<EnumValue>(value));
			context->GetScope()->add(var_name, real, context);
			return real;
		}
		context->GetScope()->add(var_name, any_cast<EnumValue>(value), context);
	}
	else if (isTypeOf<Null>(value)) {
		context->GetScope()->add(var_name, any_cast<Null>(value), context);
	}
	else if (isTypeOf<Object>(value)) {
		context->GetScope()->add(var_name, any_cast<Object>(value), context);
	}
	else {
		throw RuntimeError("Can't create varable of give type", context, node.GetStart(), node.GetEnd());
	}

	
	return value;
}

any Interpreter::visit_VarReasignNode(const VarReasignNode& node, Context* context)
{
	string var = node.GetIdentifer().GetValue().value();
	TypeToken opeation = node.GetOperationToken().GetType();
	ScopeTypes::Var value = context->GetScope()->get(var, context);
	any expr = visit(node.GetExpr(),context);

	try
	{
		switch (opeation) {
			case TypeToken::EQ: {
				using namespace InterTypes;
				if (isTypeOf<String>(expr)) {
					context->GetScope()->set(var, any_cast<String>(expr), context);
					break;
				}
				else if (isTypeOf<Integer>(expr)) {
					context->GetScope()->set(var, any_cast<Integer>(expr), context);
					break;
				}
				else if (isTypeOf<Float>(expr)) {
					context->GetScope()->set(var, any_cast<Function>(expr), context);
					break;
				}
				else if (isTypeOf<BuiltInFunction>(expr)) {
					context->GetScope()->set(var, any_cast<BuiltInFunction>(expr), context);
					break;
				}
				else if (isTypeOf<List>(expr)) {
					context->GetScope()->set(var, any_cast<List>(expr), context);
					break;
				}
				else if (isTypeOf<EnumValue>(expr)) {
					context->GetScope()->set(var, any_cast<EnumValue>(expr), context);
					break;
				}
				else if (isTypeOf<Null>(expr)) {
					context->GetScope()->set(var, any_cast<Null>(expr), context);
					break;
				}
				else if (isTypeOf<Object>(expr)) {
					context->GetScope()->set(var, any_cast<Object>(expr), context);
					break;
				}

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart(), node.GetOperationToken().GetEnd());
			}
			case TypeToken::PLUS_EQ: {
				auto* integer = get_if<Integer>(&value);
				if (integer != nullptr) {
					context->GetScope()->set(var, (*integer) + any_cast<Integer>(expr), context);
					break;
				}
				auto* flo = get_if<Float>(&value);
				if (flo != nullptr) {
					context->GetScope()->set(var, (*flo) + any_cast<Float>(expr), context);
					break;
				}
				auto* str = get_if<String>(&value);
				if (str != nullptr) {
					context->GetScope()->set(var, (*str) + any_cast<String>(expr), context);
					break;
				}

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart(), node.GetOperationToken().GetEnd());
			}
			case TypeToken::MINUS_EQ: {
				auto* integer = get_if<Integer>(&value);
				if (integer != nullptr) {
					context->GetScope()->set(var, (*integer) - any_cast<Integer>(expr), context);
					break;
				}
				auto* flo = get_if<Float>(&value);
				if (flo != nullptr) {
					context->GetScope()->set(var, (*flo) - any_cast<Float>(expr), context);
					break;
				}

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart(), node.GetOperationToken().GetEnd());
			}
			case TypeToken::DIV_EQ: {
				auto* integer = get_if<Integer>(&value);
				if (integer != nullptr) {
					context->GetScope()->set(var, (*integer) / any_cast<Integer>(expr), context);
					break;
				}
				auto* flo = get_if<Float>(&value);
				if (flo != nullptr) {
					context->GetScope()->set(var, (*flo) / any_cast<Float>(expr), context);
					break;
				}

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart(), node.GetOperationToken().GetEnd());
			}
			case TypeToken::MUL_EQ: {
				auto* integer = get_if<Integer>(&value);
				if (integer != nullptr) {
					context->GetScope()->set(var, (*integer) * any_cast<Integer>(expr), context);
					break;
				}
				auto* flo = get_if<Float>(&value);
				if (flo != nullptr) {
					context->GetScope()->set(var, (*flo) * any_cast<Float>(expr), context);
					break;
				}
			}	
			default:
				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart(), node.GetOperationToken().GetEnd());
		}
	}
	catch (const bad_any_cast&)
	{
		throw RuntimeError("Ivaild operation", context, node.GetIdentifer().GetStart(), node.GetOperationToken().GetEnd());
	}

	return Null();
}


static any PathResution(Context* ctx, const vector<string> path, int index){
	
	ScopeTypes::Var value = ctx->GetScope()->get(path.at(index), ctx);

	// enum or namespace resultion
	auto* starte = get_if<Enum>(&value);
	if (starte != nullptr) {
		return starte->get(path.at(index+1));
	}
	auto* ns = get_if<Namespace>(&value);
	if (ns != nullptr) {
		return PathResution(ns->GetNamespaceCtx(),path,index+1);
	}

	// a value 

	auto* typeA = std::get_if<Integer>(&value);
	if (typeA != nullptr) {
		return (*typeA).SetContext(ctx);
	}

	auto* typeString = get_if<String>(&value);
	if (typeString != nullptr) {
		return (*typeString).SetContext(ctx);
	}

	auto* typeB = std::get_if<Float>(&value);
	if (typeB != nullptr) {
		return (*typeB).SetContext(ctx);
	}

	auto* func = std::get_if<Function>(&value);
	if (func != nullptr) {
		return *func;
	}

	auto* list = get_if<List>(&value);
	if (list != nullptr) {
		return (*list).SetContext(ctx);
	}

	auto* builtin = get_if<BuiltInFunction>(&value);
	if (builtin != nullptr) {
		return *builtin;
	}

	auto* isEnumValue = std::get_if<EnumValue>(&value);
	if (isEnumValue != nullptr) {
		return *isEnumValue;
	}

	auto* isNull = std::get_if<Null>(&value);
	if (isNull != nullptr) {
		return *isNull;
	}

	return Null();

}

any Interpreter::visit_PathAccessNode(const PathAccessNode& node, Context* context)
{	
	vector<string> path = node.GetPath();

	if (path.size() > 0) {
		return PathResution(context,path,0);
	}
	
	return Null();
}

any Interpreter::visit_IfNode(const IfNode& node, Context* context)
{
	for (auto statment : node.GetIfStatments()) {
		any condition = visit(statment.condition,context);
		using InterTypes::isTypeOf;
		if(isTypeOf<Integer>(condition)){
			if (any_cast<Integer>(condition).isTrue()) {
				SymbolTable* ifscope = new SymbolTable();
				ifscope->setParent(context->GetScope());
				Context* ifctx = new Context("<if scope>", ifscope, context);
				return visit(statment.expr,ifctx);
			}
		}else if(isTypeOf<String>(condition)){
			if (any_cast<String>(condition).IsTrue()) {
				SymbolTable* ifscope = new SymbolTable();
				ifscope->setParent(context->GetScope());
				Context* ifctx = new Context("<if scope>", ifscope, context);
				return visit(statment.expr, ifctx);
			}
		}
		else if (isTypeOf<Null>(condition)) {}
		else {
			throw RuntimeError("Failed to create comparition", context, node.GetStart(), node.GetEnd());
		}
	}
	if (node.GetElseStatment().has_value()) {
		SymbolTable* ifscope = new SymbolTable();
		ifscope->setParent(context->GetScope());
		Context* ifctx = new Context("<else scope>", ifscope, context);
		return visit(node.GetElseStatment(), ifctx);
	}


	return Null();
}

any Interpreter::visit_ForNode(const ForNode& node, Context* context)
{
	try
	{
		Integer start_value = any_cast<Integer>(visit(node.GetStartValueNode(), context));
		Integer end_value = any_cast<Integer>(visit(node.GetEndValueNode(), context));

		int step_value = 1;

		if (node.GetStep().has_value()) {
			step_value = any_cast<Integer>(visit(node.GetStep(), context)).GetValue();
		}

		int i = start_value.GetValue();

		string var_name = node.GetVarTokenName().GetValue().value();


		SymbolTable* forscope = new SymbolTable();
		forscope->add(var_name,start_value,context);
		forscope->setParent(context->GetScope());
		Context* forctx = new Context("<for scope>", forscope, context);

			if (step_value >= 0) {
				while (i < end_value.GetValue()) {
					forctx->GetScope()->set(var_name, Integer(i), context);
					i += step_value;
					try {
						visit(node.GetBodyNode(), forctx);
					}
					catch (const FunctionReturn& r) {
						return r.GetValue();
					}
					catch (BreakEvent) {
						break;
					}
					catch (ContinueEvent) {
						continue;
					}
				}
			}
			else {
				while (i > end_value.GetValue()) {
					forctx->GetScope()->set(var_name, Integer(i), context);
					i += step_value;
					try {
						visit(node.GetBodyNode(), forctx);
					}
					catch (const FunctionReturn& r) {
						return r.GetValue();
					}
					catch (BreakEvent) {
						break;
					}
					catch (ContinueEvent) {
						continue;
					}
					
				}
			}

		delete forctx;
		delete forscope;
	}
	catch (const std::bad_any_cast&)
	{
		throw RuntimeError("Invaild start, end, or step value",context,node.GetStart(),node.GetEnd());
	}

	return Null();
}

any Interpreter::visit_WhileNode(const WhileNode& node, Context* context)
{
	SymbolTable* whilescope = new SymbolTable();
	whilescope->setParent(context->GetScope());
	Context* whilectx = new Context("<while scope>", whilescope, context);
	try {
		while (true) {
			Integer condition = any_cast<Integer>(visit(node.GetConditionNode(), whilectx));

			if (!condition.isTrue()) {
				break;
			}

			try {
				visit(node.GetBodyNode(), whilectx);
			}
			catch (const FunctionReturn& r) {
				delete whilectx;
				delete whilescope;
				return r.GetValue();
			}
			catch (BreakEvent) {
				break;
			}
			catch (ContinueEvent) {
				continue;
			}
			
		}
	}catch(const std::bad_any_cast&){
		throw RuntimeError("Invaild while loop condition", whilectx, node.GetStart(), node.GetEnd());
	}

	delete whilectx;
	delete whilescope;
	return Null();
}

any Interpreter::visit_FuncDefNode(const FuncDefNode& node, Context* context)
{
	string name = "<anonymous>";
	any body = node.GetBody();
	vector<string> arg_names;
	if (node.GetName().has_value()) {
		name = node.GetName().value().GetValue().value();
	}

	for (auto token : node.GetArgs()) {
		arg_names.push_back(token.GetValue().value());
	}

	Function func_value(body,arg_names,node.GetStart(),node.GetEnd(), name);
	func_value.SetContext(context);
	func_value.SetPostion(node.GetStart(),node.GetEnd());

	if (name != "<anonymous>") {
		context->GetScope()->add(name, func_value,context);
	}
	
	return func_value;
}

any Interpreter::visit_FuncCallNode(const FuncCallNode& node, Context* context)
{
	try
	{
		vector<any> args;
		any value_call = visit(node.GetCallNode(), context);

		if (InterTypes::isTypeOf<Function>(value_call)) {
			Function func = any_cast<Function>(value_call);

			func.SetPostion(node.GetStart(), node.GetEnd()).SetContext(context);

			for (auto node : node.GetArgNodes()) {
				args.push_back(visit(node, context));
			}

			try
			{
				return func.exceute(args);
			}
			catch (const FunctionReturn& e)
			{
				return e.GetValue();
			}
		}
		else if (InterTypes::isTypeOf<BuiltInFunction>(value_call)) {
			BuiltInFunction func = any_cast<BuiltInFunction>(value_call);

			func.SetPostion(node.GetStart(), node.GetEnd()).SetContext(context);

			for (auto node : node.GetArgNodes()) {
				args.push_back(visit(node, context));
			}

			return func.exceute(args);
		}
		else {
			throw bad_any_cast();
		}
	}
	catch (const std::bad_any_cast&)
	{
		throw RuntimeError("Expected Function call", context, node.GetStart(), node.GetEnd());
	}
}

any Interpreter::visit_StringNode(const StringNode& node, Context* context)
{
	return String(node.GetToken().GetValue().value()).SetContext(context).SetPostion(node.GetStart(),node.GetEnd());
}

any Interpreter::visit_ListNode(const ListNode& node, Context* context)
{
	vector<any> elements;

	for (auto el : node.GetElements()) {
		if (el.has_value()) {
			elements.push_back(visit(el, context));
		}
	}

	return List(elements).SetContext(context).SetPostion(node.GetStart(),node.GetEnd());
}

any Interpreter::visit_ListAccessNode(const ListAccessNode& node, Context* context)
{
	using namespace InterTypes;
	try
	{
		Integer index = any_cast<Integer>(visit(node.GetIndex(), context));

		List list = get<List>(context->GetScope()->get(node.GetVarName(),context));

		return list.GetElement(index);

	}
	catch (std::bad_variant_access const&) {
		throw RuntimeError("Variable was not a list", context, node.GetStart(), node.GetEnd());
	}
	catch (const std::bad_any_cast&)
	{
		throw RuntimeError("Invaild list index", context, node.GetStart(), node.GetEnd());
	}
}

any Interpreter::visit_ReturnNode(const ReturnNode& node, Context* context)
{
	throw FunctionReturn(visit(node.GetReturnValue(),context));
}

any Interpreter::visit_EnumDefNode(const EnumNode& node, Context* context)
{
	Enum def(node.GetKeys(), node.GetName());
	def.SetContext(context).SetPostion(node.GetStart(), node.GetEnd());

	context->GetScope()->add(node.GetName(),def,context);

	return def;
}

any Interpreter::visit_NamespaceNode(const NamespaceNode& node, Context* context)
{
	string nameNS = node.GetName().GetValue().value();
	SymbolTable* scopeNS = new SymbolTable();
	scopeNS->setParent(context->GetScope());
	Context* ctxNS = new Context(nameNS,scopeNS,context,node.GetStart());

	if (node.GetStatements().has_value()) {
		visit(node.GetStatements(), ctxNS);
	}

	Namespace def(nameNS, ctxNS);
	def.SetContext(context).SetPostion(node.GetStart(), node.GetEnd());
	context->GetScope()->add(nameNS,def,context);

	return def;
}

any Interpreter::visit_ImportNode(const ImportNode& node, Context* context)
{
	string file = node.GetPath();
	string text;
	//https://raw.githubusercontent.com/VisualSource/Polyscript/master/Polyscript/std.ps?token=AKIUJD5ON2SBT5KWPY3N6BDAUQW7U
	// do a std libary import check here
	if (file.find("https://",0,8) != string::npos) {

		httplib::Result res = fetch::Get(file,context);

		text = res->body;
	}
	else {
		std::ifstream reader(file);
		if (!reader.is_open()) throw RuntimeError("Failed to open import: " + file, context, node.GetStart(), node.GetEnd());
		text = string((std::istreambuf_iterator<char>(reader)), (std::istreambuf_iterator<char>()));
		reader.close();
	}

	Lexer lexer(text, file);
	Parser parser(lexer.makeTokens());
	
	return visit(parser.parse(),context);
}

any Interpreter::visit_MatchNode(const MatchNode& node, Context* context)
{
	using InterTypes::isTypeOf;
	any value = visit(node.GetExpr(),context);

	try{
		if (isTypeOf<Integer>(value)) {
			Integer incoming = any_cast<Integer>(value);
			vector<MatchExpr> matches = node.GetMatches();

			size_t index = 0;
			while (index < matches.size()) {
				SymbolTable* matchScope = nullptr;
				Context* ctx = context;
				try {
					// scoped matches
					if (matches.at(index).scoped) {
						matchScope = new SymbolTable();
						matchScope->setParent(context->GetScope());
						ctx = new Context("<match scope>", matchScope, context, node.GetStart());
					}

					if (isTypeOf<MatchCatchAll>(matches.at(index).checkValue)) {
						any out = visit(matches.at(index).expr, ctx);
						if (matches.at(index).scoped) {
							delete matchScope;
							delete ctx;
						}
						return out;
					}
					Integer check = any_cast<Integer>(visit(matches.at(index).checkValue, context));

					if ((incoming == check).GetValue()) {
						any out = visit(matches.at(index).expr, ctx);
						if (matches.at(index).scoped) {
							delete matchScope;
							delete ctx;
						}
						return out;
					}
				}
				catch (const FunctionReturn& e) {
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return e.GetValue();
				}
				catch (BreakEvent) {
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return Null().SetPostion(node.GetStart(), node.GetEnd());;
				}
				catch (ContinueEvent) {
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return Null().SetPostion(node.GetStart(), node.GetEnd());;
				}
				index++;
			}

		}
		else if (isTypeOf<String>(value)) {
			String incoming = any_cast<String>(value);
			vector<MatchExpr> matches = node.GetMatches();

			size_t index = 0;
			while (index < matches.size()) {
				SymbolTable* matchScope = nullptr;
				Context* ctx = context;
				try {
					// scoped matches
					if (matches.at(index).scoped) {
						matchScope = new SymbolTable();
						matchScope->setParent(context->GetScope());
						ctx = new Context("<match scope>", matchScope, context, node.GetStart());
					}

					if (isTypeOf<MatchCatchAll>(matches.at(index).checkValue)) {
						any out = visit(matches.at(index).expr, ctx);
						if (matches.at(index).scoped) {
							delete matchScope;
							delete ctx;
						}
						return out;
					}
					String check = any_cast<String>(visit(matches.at(index).checkValue, context));

					if ((incoming == check).GetValue()) {
						any out = visit(matches.at(index).expr, ctx);
						if (matches.at(index).scoped) {
							delete matchScope;
							delete ctx;
						}
						return out;
					}
				}
				catch (const FunctionReturn& e) {
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return e.GetValue();
				}
				catch (BreakEvent) {
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return Null().SetPostion(node.GetStart(), node.GetEnd());;
				}
				catch (ContinueEvent) {
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return Null().SetPostion(node.GetStart(), node.GetEnd());;
				}
				index++;
			}

		}
		else if (isTypeOf<Float>(value)) {
			Float incoming = any_cast<Float>(value);
			vector<MatchExpr> matches = node.GetMatches();

			size_t index = 0;
			while (index < matches.size()) {
				SymbolTable* matchScope = nullptr;
				Context* ctx = context;
				try {
					// scoped matches
					if (matches.at(index).scoped) {
						matchScope = new SymbolTable();
						matchScope->setParent(context->GetScope());
						ctx = new Context("<match scope>", matchScope, context, node.GetStart());
					}

					if (isTypeOf<MatchCatchAll>(matches.at(index).checkValue)) {
						any out = visit(matches.at(index).expr, ctx);
						if (matches.at(index).scoped) {
							delete matchScope;
							delete ctx;
						}
						return out;
					}
					Float check = any_cast<Float>(visit(matches.at(index).checkValue, context));

					if ((incoming == check).GetValue()) {
						any out = visit(matches.at(index).expr, ctx);
						if (matches.at(index).scoped) {
							delete matchScope;
							delete ctx;
						}
						return out;
					}
				}catch (const FunctionReturn& e) {
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return e.GetValue();
				} 
				catch(BreakEvent){
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return Null().SetPostion(node.GetStart(), node.GetEnd());;
				}
				catch(ContinueEvent){
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return Null().SetPostion(node.GetStart(), node.GetEnd());;
				}
				index++;
			}

		}
		else if (isTypeOf<EnumValue>(value)) {
		EnumValue incoming = any_cast<EnumValue>(value);
		vector<MatchExpr> matches = node.GetMatches();

		size_t index = 0;
		while (index < matches.size()) {
			SymbolTable* matchScope = nullptr;
			Context* ctx = context;
			try {
				// scoped matches
				if (matches.at(index).scoped) {
					matchScope = new SymbolTable();
					matchScope->setParent(context->GetScope());
					ctx = new Context("<match scope>", matchScope, context, node.GetStart());
				}

				if (isTypeOf<MatchCatchAll>(matches.at(index).checkValue)) {
					any out = visit(matches.at(index).expr, ctx);
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return out;
				}
				EnumValue check = any_cast<EnumValue>(visit(matches.at(index).checkValue, context));

				if ((incoming == check).GetValue()) {
					any out = visit(matches.at(index).expr, ctx);
					if (matches.at(index).scoped) {
						delete matchScope;
						delete ctx;
					}
					return out;
				}
			}
			catch (const FunctionReturn& e) {
				if (matches.at(index).scoped) {
					delete matchScope;
					delete ctx;
				}
				return e.GetValue();
			}
			catch (BreakEvent) {
				if (matches.at(index).scoped) {
					delete matchScope;
					delete ctx;
				}
				return Null();
			}
			catch (ContinueEvent) {
				if (matches.at(index).scoped) {
					delete matchScope;
					delete ctx;
				}
				return Null();
			}
			index++;
		}

		}
		else {
			throw RuntimeError("Can not match type", context, node.GetStart(), node.GetEnd());
		}
	}
	catch (const std::bad_any_cast&)
	{
		throw RuntimeError("Match statment does not match expr", context, node.GetStart(), node.GetEnd());
	}


	return Null();
}

std::any Interpreter::visit_ObjectNode(const ObjectNode& node, Context* context)
{
	vector<ObjectProperties> props;

	for (auto prop : node.GetValues()) {
		props.push_back(ObjectProperties { prop.name.GetValue().value(), visit(prop.expr,context) });
	}

	return Object(props,context).SetPostion(node.GetStart(), node.GetEnd());
}


static any DotPathResution(Object obj, Context* ctx, const vector<string> path, int index) {

	any value = obj.GetValue(path.at(index));

	if (InterTypes::isTypeOf<Object>(value)) {
		return DotPathResution(any_cast<Object>(value), ctx, path, index + 1);
	}

	return value;

}

std::any Interpreter::visit_DotAccessNode(const DotAccessNode& node, Context* context)
{
	try {
		vector<string> path = node.GetAccessPath();
		Object owner = get<Object>(context->GetScope()->get(path.at(0), context));
		return DotPathResution(owner, context, path, 1);
	}
	catch (const std::bad_variant_access&)
	{
		throw RuntimeError("Identfier does not point to a Object", context, node.GetStart(), node.GetEnd());
	}


	return Null().SetContext(context).SetPostion(node.GetStart(),node.GetEnd());
}

FunctionReturn::FunctionReturn(any value): value(value)
{
}

any FunctionReturn::GetValue() const
{
	return value;
}
