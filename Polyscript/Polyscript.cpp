#include <iostream>
#include <fstream>
#include <string>
#include <any>
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



using namespace std;


void run(string fn, string text, SymbolTable* scope, bool showTokens){
	try {
		Lexer lexer(text,fn);
		vector<Token> tokens = lexer.makeTokens();

		if (showTokens) {
			cout << "TOKENS [ ";
			for (auto token : tokens) { cout << token << " "; }
			cout << "]" << endl;
		}

		Parser parser(tokens);

		ParseResult ast = parser.parse();

		if (ast.hasErr()) {
			throw ast.GetErr();
		}

		Context* context = new Context("<program>", scope);
		Interpreter interpreter;
		any result = interpreter.visit(ast.GetNode(),context);
		InterTypes::print(cout,result);
		cout << endl;
		delete context;

	} catch (IllegalCharError e) {
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
		cerr << e.what() << endl;
	}
	catch (logic_error e) {
		cerr << e.what() << endl;
	}
	catch (...) {
		cerr << "Unkown Runtime Error" << endl;
	}
}

int main(int argc, char* argv[]) {
	bool showTokens = false;
	string file_name;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--showTokens")) showTokens = true;
	}

	SymbolTable* globalscope = new SymbolTable();
	globalscope->add("__name__", Integer(0));
	globalscope->add("null", Integer(0));
	globalscope->add("false", Integer(0));
	globalscope->add("true", Integer(1));

		cout << "Polyscript \x1B[94mV0.1.0\033[0m | use exit() to exit." << endl;

		while (true) {
			cout << "> ";
			string input;
			getline(cin, input);
			if (input == "exit()") break;
			run("<stdin>", input, globalscope, showTokens);
		}
	

	return 0;
}


