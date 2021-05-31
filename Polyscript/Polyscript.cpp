#include <iostream>
#include <fstream>
#include <string>
#include <any>
#include "InterpreterUtils.h"
#include "SymbolTable.h"
#include "Context.h"
#include "IllegalCharError.h"
#include "InvalidSyntaxError.h"
#include "ExpectedCharError.h"
#include "RuntimeError.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "NodeUtils.h"
#include "BuiltInFunction.h"

void run(std::string fn, std::string text, SymbolTable* scope, bool showTokens, bool fromFile) {
	using namespace std;
	try {
		Lexer lexer(text, fn);
		vector<Token> tokens = lexer.makeTokens();

		if (showTokens) {
			cout << "TOKENS [ ";
			for (auto token : tokens) { cout << token << " "; }
			cout << "]" << endl;
		}

		Parser parser(tokens);

		any ast = parser.parse();

		Context* context = new Context("<program>", scope);
		Interpreter interpreter;
		try
		{
			List result = any_cast<List>(interpreter.visit(ast, context));
			if (!fromFile) {
				if (result.GetElements().size() != 0) {
					InterTypes::print(cout, result.GetElement(result.GetElements().size() - 1));
				}
			}
		}
		catch (const FunctionReturn& r)
		{
			if (!fromFile) InterTypes::print(cout, r.GetValue());
		}
		catch (BreakEvent) {}
		catch (ContinueEvent) {}
		if (!fromFile) cout << endl;
		delete context;

	}
	catch (IllegalCharError e) {
		cerr << e << endl;
	}
	catch (InvalidSyntaxError e) {
		cerr << e << endl;
	}
	catch (RuntimeError e) {
		cerr << e << endl;
	}
	catch (ExpectedCharError e) {
		cerr << e << endl;
	}
	catch (PolyscriptError e) {
		cerr << e << endl;
	}
	catch (runtime_error e) {
		cerr << "Runtime Error: " + std::string(e.what()) << endl;
	}
	catch (out_of_range e) {
		cerr << "Runtime out of range error: " + std::string(e.what()) << endl;
	}
	catch (logic_error e) {
		cerr << "Runtime logic error: " + std::string(e.what()) << endl;
	}
	catch (...) {
		cerr << "Unname Runtime Error" << endl;
	}
}
void read_run(std::string file, SymbolTable* scope, bool showTokens){
	std::ifstream reader(file);
	if (!reader.is_open()) return;
	std::string text((std::istreambuf_iterator<char>(reader)), (std::istreambuf_iterator<char>()));
	reader.close();

	run(file,text,scope,showTokens,true);
}


int main(int argc, char* argv[]) {
	using namespace std;
	bool showTokens = false;
	bool fromFile = false;
	string file_name;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "run")) {
			fromFile = true;
			i++;
			file_name = argv[i];
		}
		if (!strcmp(argv[i], "--showTokens")) showTokens = true;

	}

	vector<string> bulitin = { "__input" };
	vector<string> fetchParams = { "__url" };
	SymbolTable* globalscope = new SymbolTable();
	globalscope->add("__name__", String("main"), nullptr, false);
	globalscope->add("null", Null(), nullptr, false);
	globalscope->add("false", Integer::False(), nullptr, false);
	globalscope->add("true", Integer::True(), nullptr, false);
	globalscope->add("print", BuiltInFunction("print", bulitin),nullptr, false);
	globalscope->add("clear", BuiltInFunction("clear", vector<string>()), nullptr, false);
	globalscope->add("isInteger", BuiltInFunction("isInteger", bulitin), nullptr, false);
	globalscope->add("isFloat", BuiltInFunction("isFloat", bulitin), nullptr, false);
	globalscope->add("isFunction", BuiltInFunction("print", bulitin), nullptr, false);
	globalscope->add("isList", BuiltInFunction("isList", bulitin), nullptr, false);
	globalscope->add("isString", BuiltInFunction("isString", bulitin), nullptr, false);
	globalscope->add("isEnum", BuiltInFunction("isEnum", bulitin), nullptr, false);
	globalscope->add("isNull", BuiltInFunction("isNull", bulitin), nullptr, false);
	globalscope->add("length", BuiltInFunction("length", bulitin), nullptr, false);
	globalscope->add("syncFetch", BuiltInFunction("syncFetch", fetchParams), nullptr,false);

	if (!fromFile) {
		cout << "Polyscript \x1B[94mV0.4.0\033[0m | use exit() to exit." << endl;

		while (true) {
			cout << "> ";
			string input;
			getline(cin, input);
			if (input == "exit()") break;
			if (input.size() == 0) {
				cout << "\x1B[90mnull\033[0m" << endl;
				continue;
			}
			run("<stdin>", input, globalscope, showTokens, false);
		}
	}
	else {
		read_run(file_name,globalscope,showTokens);
	}

		
	

	return 0;
}


