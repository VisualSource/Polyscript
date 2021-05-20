#pragma once
#include <string>
#include <any>
#include <vector>
#include <ostream>
#include "Variable.h"

class BaseFunction: public Variable {
	public:
		BaseFunction(std::string name = "<anonymous>");
		~BaseFunction();
		virtual BaseFunction& SetContext(Context* context) override;
		virtual BaseFunction& SetPostion(Position start, Position end) override;
		virtual std::any exceute(std::vector<std::any> args) = 0;
		void check_and_populate(const std::vector<std::string>& arg_names, const std::vector<std::any>& args, Context* ctx);
		Context* NewContext();
		std::string GetName() const { return name; };
		friend std::ostream& operator<<(std::ostream& lhs, const  BaseFunction& other);
	private:
		void check_args(const std::vector<std::string>& arg_names, const std::vector<std::any>& args);
		void populate_args(const std::vector<std::string>& arg_names, const std::vector<std::any>& args, Context* ctx);
		std::string name;
		Context* functionScope = nullptr;
};

