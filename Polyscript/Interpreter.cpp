#include "Interpreter.h"
#include <string>
#include <variant>
#include "InterpreterUtils.h"
#include "SymbolTable.h"
#include "PolyscriptError.h"
#include "RuntimeError.h"
#include "Nodes.h"

Interpreter::Interpreter() {
}


any Interpreter::visit(const any& node, Context* context) {
	using namespace NodeUtils;
	if (isBinOpNode(node)) {
		return visit_BinOpNode(any_cast<BinOpNode>(node), context);
	}
	else if (isUnaryOpNode(node)) {
		return visit_UnaryOpNode(any_cast<UnaryOpNode>(node), context);
	}
	else if (isNumberNode(node)) {
		return visit_NumberNode(any_cast<NumberNode>(node), context);
	}
	else if (isAssignNode(node)) {
		return visit_VarAssignNode(any_cast<VarAssignNode>(node), context);
	}
	else if (isVarAccessNode(node)) {
		return visit_VarAccessNode(any_cast<VarAccessNode>(node), context);
	}
	else if (isIfNode(node)) {
		return visit_IfNode(any_cast<IfNode>(node), context);
	}
	else if (isForNode(node)) {
		return visit_ForNode(any_cast<ForNode>(node), context);
	}
	else if (isWhileNode(node)) {
		return visit_WhileNode(any_cast<WhileNode>(node), context);
	}
	else if (isFuncDefNode(node)) {
		return visit_FuncDefNode(any_cast<FuncDefNode>(node), context);
	}else if(isFuncCallNode(node)){
		return visit_FuncCallNode(any_cast<FuncCallNode>(node), context);
	} else if(isStringNode(node)){
		return visit_StringNode(any_cast<StringNode>(node), context);
	}
	else if (isListNode(node)) {
		return visit_ListNode(any_cast<ListNode>(node), context);
	}
	else if (isListAccessNode(node)) {
		return visit_ListAccessNode(any_cast<ListAccessNode>(node), context);
	}
	else if (isReturnNode(node)) {
		return visit_ReturnNode(any_cast<ReturnNode>(node),context);
	}
	else if (isBreakNode(node)) {
		throw BreakEvent();
	}
	else if (isContinueNode(node)) {
		throw ContinueEvent();
	}
	else if (isEnumNode(node)) {
		return visit_EnumDefNode(any_cast<EnumNode>(node),context);
	}
	else if (isPathAccessNode(node)) {
		return visit_PathAccessNode(any_cast<PathAccessNode>(node), context);
	}
	else if (isVarReasignNode(node)) {
		return visit_VarReasignNode(any_cast<VarReasignNode>(node), context);
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
		number.SetPostion(n.GetStart().value(),n.GetEnd().value());
		return number;
	} else if (n.isToken(TypeToken::FLOAT)) {
		Float number(std::stod(n.GetValue().value()));
		number.SetPostion(n.GetStart().value(), n.GetEnd().value());
		number.SetContext(context);
		return number;
	}
	else {
		throw PolyscriptError("Invaild number","Can't parse number",node.GetStart(),node.GetEnd());
	}

}

any Interpreter::visit_BinOpNode(const BinOpNode& node, Context* context) {
	using InterTypes::isFloat;
	using InterTypes::isInteger;
	using InterTypes::isString;
	any left = visit(node.GetLeftNode(),context);
	any right = visit(node.GetRightNode(),context);

	switch (node.GetOpToken().GetType()) {
		case TypeToken::PLUS: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat + rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt + rightInt;
			}
			else if (isString(left) && isString(right)) {
				String leftInt = any_cast<String>(left);
				String rightInt = any_cast<String>(right);
				return leftInt + rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(),node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::MINUS: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat - rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt - rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::MUL: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat * rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt * rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::DIV: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat / rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt / rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::POWER: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat.power(rightFloat);
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt.power(rightInt);
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::EE: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat == rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt == rightInt;
			}
			else if (isString(left) && isString(right)) {
				String leftInt = any_cast<String>(left);
				String rightInt = any_cast<String>(right);
				return leftInt == rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::NE: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat != rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt != rightInt;
			}
			else if (isString(left) && isString(right)) {
				String leftInt = any_cast<String>(left);
				String rightInt = any_cast<String>(right);
				return leftInt != rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::LT: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat < rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt < rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::GT: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat > rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt > rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::LTE: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat >= rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt >= rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::GTE: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return leftFloat <= rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt <= rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::AND: {
			if (isFloat(left) && isFloat(right)) {
				Integer leftFloat = Integer::cast(any_cast<Float>(left));
				Integer rightFloat = Integer::cast(any_cast<Float>(right));
				return leftFloat && rightFloat;
			}else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt && rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::OR: {
			if (isFloat(left) && isFloat(right)) {
				Integer leftFloat = Integer::cast(any_cast<Float>(left));
				Integer rightFloat = Integer::cast(any_cast<Float>(right));
				return leftFloat || rightFloat;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt || rightInt;
			}
			else {
				throw RuntimeError("Invaild Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		default:
			throw RuntimeError("Undefined Operation", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
	}
}

any Interpreter::visit_UnaryOpNode(const UnaryOpNode& node, Context* context) {
	using InterTypes::isFloat;
	using InterTypes::isInteger;
	any number = visit(node.GetNode(),context);

	if (node.GetToken().isToken(TypeToken::MINUS)) {
		if (isFloat(number)) {
			Float nodeF = any_cast<Float>(number);
			return nodeF * Float(-1.0);
		} else if (isInteger(number)) {
			Integer nodeI = any_cast<Integer>(number);
			return nodeI * Integer(-1);
		}else {
			string className = number.type().name();
			throw RuntimeError("Invailed number type: " + className, context, node.GetToken().GetStart().value(), node.GetToken().GetEnd().value());
		}
	}
	else if (node.GetToken().isToken(TypeToken::NOT)) {
		if (isFloat(number)) {
			Float nodeF = any_cast<Float>(number);
			return !Integer::cast(nodeF);
		}
		else if (isInteger(number)) {
			Integer nodeI = any_cast<Integer>(number);
			return !nodeI;
		}
		else {
			string className = number.type().name();
			throw RuntimeError("Invailed type: " + className, context, node.GetToken().GetStart().value(), node.GetToken().GetEnd().value());
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

	throw RuntimeError("Failed to access variable", context,node.GetStart(),node.GetEnd());
}

any Interpreter::visit_VarAssignNode(const VarAssignNode& node, Context* context)
{
	string var_name = node.GetToken().GetValue().value();

	any value = visit(node.GetNode(),context);

	
	if (InterTypes::isInteger(value)) {
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
	else if (InterTypes::isFloat(value)) {
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
	else if (InterTypes::isString(value)) {
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
	else if (InterTypes::isList(value)) {
		context->GetScope()->add(var_name, any_cast<List>(value),context);
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
				if (isString(expr)) {
					context->GetScope()->set(var, any_cast<String>(expr), context);
					break;
				}
				else if (isInteger(expr)) {
					context->GetScope()->set(var, any_cast<Integer>(expr), context);
					break;
				}
				else if (isFloat(expr)) {
					context->GetScope()->set(var, any_cast<Function>(expr), context);
					break;
				}
				else if (isBuiltin(expr)) {
					context->GetScope()->set(var, any_cast<BuiltInFunction>(expr), context);
					break;
				}
				else if (isList(expr)) {
					context->GetScope()->set(var, any_cast<List>(expr), context);
					break;
				}

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart().value(), node.GetOperationToken().GetEnd().value());
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

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart().value(), node.GetOperationToken().GetEnd().value());
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

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart().value(), node.GetOperationToken().GetEnd().value());
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

				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart().value(), node.GetOperationToken().GetEnd().value());
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
				throw RuntimeError("Invaild Opeation", context, node.GetIdentifer().GetStart().value(), node.GetOperationToken().GetEnd().value());
		}
	}
	catch (const bad_any_cast&)
	{
		throw RuntimeError("Ivaild operation", context, node.GetIdentifer().GetStart().value(), node.GetOperationToken().GetEnd().value());
	}

	return any();
}

any Interpreter::visit_PathAccessNode(const PathAccessNode& node, Context* context)
{	
	vector<string> path = node.GetPath();
	if (path.size() > 0) {
		string start = path.at(0);
		ScopeTypes::Var value = context->GetScope()->get(start, context);
		auto* starte = get_if<Enum>(&value);
		if (starte != nullptr) {
			 return starte->get(path.at(1));
		}
	}
	
	return any();
}

any Interpreter::visit_IfNode(const IfNode& node, Context* context)
{
	for (auto statment : node.GetIfStatments()) {
		any condition = visit(statment.condition,context);

		if(InterTypes::isInteger(condition)){
			if (any_cast<Integer>(condition).isTrue()) {
				SymbolTable* ifscope = new SymbolTable();
				ifscope->setParent(context->GetScope());
				Context* ifctx = new Context("<if scope>", ifscope, context);
				return visit(statment.expr,ifctx);
			}
		}else if(InterTypes::isString(condition)){
			if (any_cast<String>(condition).IsTrue()) {
				SymbolTable* ifscope = new SymbolTable();
				ifscope->setParent(context->GetScope());
				Context* ifctx = new Context("<if scope>", ifscope, context);
				return visit(statment.expr, ifctx);
			}
		}
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


	return any();
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

	return any();
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
	return any();
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

	Function func_value(body,arg_names,name);
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

		if (InterTypes::isFunction(value_call)) {
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
		else if (InterTypes::isBuiltin(value_call)) {
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
		elements.push_back(visit(el, context));
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

FunctionReturn::FunctionReturn(any value): value(value)
{
}

any FunctionReturn::GetValue() const
{
	return value;
}
