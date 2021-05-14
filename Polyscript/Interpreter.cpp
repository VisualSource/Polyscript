#include "Interpreter.h"
#include <string>
#include <variant>
#include "Integer.h"
#include "Float.h"
#include "PolyscriptError.h"
#include "RuntimeError.h"
#include "SymbolTable.h"

bool InterTypes::isFloat(const any& node){
	return node.type() == typeid(Float);
}
bool InterTypes::isInteger(const any& node) {
	return node.type() == typeid(Integer);
}

void InterTypes::print(ostream& rhs, const any& node) {
	if (InterTypes::isFloat(node)) {
		rhs << any_cast<Float>(node);
	}else if(InterTypes::isInteger(node)){
		rhs << any_cast<Integer>(node);
	}
}

Interpreter::Interpreter() {
}


any Interpreter::visit(const any& node, Context* context) {
	if (NodeUtils::isBinOpNode(node)) {
		return visit_BinOpNode(any_cast<BinOpNode>(node), context);
	}
	else if (NodeUtils::isUnaryOpNode(node)) {
		return visit_UnaryOpNode(any_cast<UnaryOpNode>(node), context);
	}
	else if (NodeUtils::isNumberNode(node)) {
		return visit_NumberNode(any_cast<NumberNode>(node), context);
	}
	else if (NodeUtils::isAssignNode(node)) {
		return visit_VarAssignNode(any_cast<VarAssignNode>(node), context);
	}
	else if (NodeUtils::isVarAccessNode(node)) {
		return visit_VarAccessNode(any_cast<VarAccessNode>(node), context);
	}
	else if (NodeUtils::isIfNode(node)) {
		return visit_IfNode(any_cast<IfNode>(node), context);
	}
	else {
		throw PolyscriptError("Undefined Operation", "No Visit Method", Position(), Position());
	}

}

any Interpreter::visit_NumberNode(const NumberNode& node, Context* context) {
	Token n = node.GetToken();
	if (n.isToken(TypeToken::INT)) {
		Integer number(std::stoi(n.GetValue().value()));
		number.set_context(context);
		number.set_pos(n.GetStart(),n.GetEnd());
		return number;
	} else if (n.isToken(TypeToken::FLOAT)) {
		Float number(std::stod(n.GetValue().value()));
		number.set_pos(n.GetStart(), n.GetEnd());
		number.set_context(context);
		return number;
	}
	else {
		throw PolyscriptError("Invaild number","Can't parse number",node.GetStart(),node.GetEnd());
	}

}

any Interpreter::visit_BinOpNode(const BinOpNode& node, Context* context) {
	using InterTypes::isFloat;
	using InterTypes::isInteger;
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
			} else {
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(),node.GetOpToken().GetEnd().value());
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
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
			else {
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
			else {
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::LT: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return Integer(int(leftFloat < rightFloat)).set_context(context);;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt < rightInt;
			}
			else {
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::GT: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return Integer(int(leftFloat > rightFloat)).set_context(context);;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt > rightInt;
			}
			else {
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::LTE: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return Integer(int(leftFloat >= rightFloat)).set_context(context);;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt >= rightInt;
			}
			else {
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		case TypeToken::GTE: {
			if (isFloat(left) && isFloat(right)) {
				Float leftFloat = any_cast<Float>(left);
				Float rightFloat = any_cast<Float>(right);
				return Integer(int(leftFloat <= rightFloat)).set_context(context);;
			}
			else if (isInteger(left) && isInteger(right)) {
				Integer leftInt = any_cast<Integer>(left);
				Integer rightInt = any_cast<Integer>(right);
				return leftInt <= rightInt;
			}
			else {
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
				throw RuntimeError("Left does not match right", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
			}
		}
		default:
			throw RuntimeError("Unknow Operator", context, node.GetOpToken().GetStart().value(), node.GetOpToken().GetEnd().value());
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
	Var value = context->GetScope()->get(var_name);

	auto* typeA = std::get_if<Integer>(&value);
	if (typeA != nullptr) {
		return *typeA;
	}

	auto* typeB = std::get_if<Float>(&value);
	if (typeB != nullptr) {
		return *typeB;
	}

	return value;

}

any Interpreter::visit_VarAssignNode(const VarAssignNode& node, Context* context)
{
	string var_name = node.GetToken().GetValue().value();

	any value = visit(node.GetNode(),context);

	
	if (InterTypes::isInteger(value)) {
		if (node.GetToken().GetType() != node.GetVarType()) {
			Float realF = Float::cast(any_cast<Integer>(value));
			context->GetScope()->add(var_name, realF);
			return realF;
		} else {
			context->GetScope()->add(var_name, any_cast<Integer>(value));
		}
	}
	else if (InterTypes::isFloat(value)) {
			if (node.GetToken().GetType() != node.GetVarType()) {
				Integer realF = Integer::cast(any_cast<Float>(value));
				context->GetScope()->add(var_name, realF);
				return realF;
			}
			else {
				context->GetScope()->add(var_name, any_cast<Float>(value));
			}
	}
	else {
		throw RuntimeError("Can't create varable of give type", context, node.GetStart(), node.GetEnd());
	}

	
	return value;
}

any Interpreter::visit_IfNode(const IfNode& node, Context* context)
{
	for (auto statment : node.GetIfStatments()) {
		any condition = visit(statment.condition,context);

		if(InterTypes::isInteger(condition)){
			if (any_cast<Integer>(condition).isTrue()) {
				SymbolTable* ifscope = new SymbolTable();
				ifscope->setParent(context->GetScope());
				Context* ifctx = new Context("if", ifscope, context);
				return visit(statment.expr,ifctx);
			}
		}else {
			throw RuntimeError("Failed to create comparition",context,node.GetStart(),node.GetEnd());
		}
	}
	if (node.GetElseStatment().has_value()) {
		SymbolTable* ifscope = new SymbolTable();
		ifscope->setParent(context->GetScope());
		Context* ifctx = new Context("else", ifscope, context);
		return visit(node.GetElseStatment(), ifctx);
	}


	return any();
}
