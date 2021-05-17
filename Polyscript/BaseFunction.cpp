#include "BaseFunction.h"
#include "RuntimeError.h"
#include "Context.h"
#include "SymbolTable.h"
#include "InterpreterUtils.h"

BaseFunction::BaseFunction(string name): name(name)
{
}

BaseFunction::~BaseFunction()
{
	if (functionScope != nullptr) {
		delete functionScope;
	}
}

BaseFunction& BaseFunction::SetContext(Context* context)
{
	this->context = context;
	return *this;
}

BaseFunction& BaseFunction::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}

void BaseFunction::check_args(const vector<string>& arg_names, const vector<any>& args)
{
	if (args.size() > arg_names.size()) {
		throw RuntimeError("Too many args passed into " + this->name, context, this->start, this->end);
	}
	if (args.size() < arg_names.size()) {
		throw RuntimeError("Too few args passed into " + this->name, context, this->start, this->end);
	}
}

void BaseFunction::populate_args(const vector<string>& arg_names, const vector<any>& args, Context* ctx)
{
	using InterTypes::isTypeOf;
	for (size_t i = 0; i < args.size(); i++) {
		string arg_name = arg_names.at(i);
		any arg_value = args.at(i);

		if (isTypeOf<Integer>(arg_value)) {
			Integer iv = any_cast<Integer>(arg_value);
			iv.SetContext(ctx);
			ctx->GetScope()->add(arg_name, iv,ctx);
		}
		else if (isTypeOf<Float>(arg_value)) {
			Float fl = any_cast<Float>(arg_value);
			fl.SetContext(ctx);
			ctx->GetScope()->add(arg_name, fl, ctx);
		}
		else if (isTypeOf<String>(arg_value)) {
			String fl = any_cast<String>(arg_value);
			fl.SetContext(ctx);
			ctx->GetScope()->add(arg_name, fl, ctx);
		}
		else if (isTypeOf<Function>(arg_value)) {
			Function fl = any_cast<Function>(arg_value);
			fl.SetContext(ctx);
			ctx->GetScope()->add(arg_name, fl, ctx);
		}
		else if (isTypeOf<List>(arg_value)) {
			List fl = any_cast<List>(arg_value);
			fl.SetContext(ctx);
			ctx->GetScope()->add(arg_name, fl, ctx);
		}
		else if (isTypeOf<EnumValue>(arg_value)) {
			EnumValue fl = any_cast<EnumValue>(arg_value);
			fl.SetContext(ctx);
			ctx->GetScope()->add(arg_name, fl, ctx);
		}
		else if (isTypeOf<Null>(arg_value)) {
			Null fl = any_cast<Null>(arg_value);
			fl.SetContext(ctx);
			ctx->GetScope()->add(arg_name, fl, ctx);
		}
		else {
			throw RuntimeError("Failed to create function argument", ctx, this->start, this->end);
		}
	}
}

void BaseFunction::check_and_populate(const vector<string>& arg_names, const vector<any>& args, Context* ctx)
{
	check_args(arg_names, args);
	populate_args(arg_names, args, ctx);
}

Context* BaseFunction::NewContext()
{
	SymbolTable* scope = new SymbolTable();
	scope->setParent(this->context->GetScope());
	Context* ctx = new Context(this->name,scope,this->context,this->start);
	return ctx;
}

ostream& operator<<(ostream& lhs, const BaseFunction& other)
{
	lhs << "<\x1B[36mfunction " << other.name << "\033[0m>";
	return lhs;
}
