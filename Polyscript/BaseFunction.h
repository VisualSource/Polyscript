#pragma once
#include <string>
#include <any>
#include <vector>
#include <ostream>
#include "Variable.h"

using namespace std;

class BaseFunction: public Variable {
	public:
		BaseFunction(string name = "<anonymous>");
		~BaseFunction();
		virtual BaseFunction& SetContext(Context* context) override;
		virtual BaseFunction& SetPostion(Position start, Position end) override;
		virtual any exceute(vector<any> args) = 0;
		void check_and_populate(const vector<string>& arg_names, const vector<any>& args, Context* ctx);
		Context* NewContext();
		string GetName() const { return name; };
		friend ostream& operator<<(ostream& lhs, const  BaseFunction& other);
	private:
		void check_args(const vector<string>& arg_names, const vector<any>& args);
		void populate_args(const vector<string>& arg_names, const vector<any>& args, Context* ctx);
		string name;
		Context* functionScope = nullptr;
};

