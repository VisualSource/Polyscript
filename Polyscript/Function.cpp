#include "Function.h"
#include "Interpreter.h"
#include "InterpreterUtils.h"
#include "Context.h"
#include "SymbolTable.h"
#include "RuntimeError.h"
#include "Float.h"
#include "List.h"
#include "Integer.h"
#include "NodeUtils.h"

using namespace std;


Function::Function(any body_node, vector<string> arg_names, Position start, Position end, string name): body_node(body_node), arg_names(arg_names), BaseFunction(name)
{
	this->end = end;
	this->start = start;
}

any Function::exceute(vector<any> args)
{
	Interpreter interperter;
	Context* ctx = NewContext();

	check_and_populate(arg_names, args,ctx);
	
	return interperter.visit(this->body_node,ctx);

}
