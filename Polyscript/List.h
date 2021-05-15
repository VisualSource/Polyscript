#pragma once
#include "Variable.h"
#include <vector>
#include <any>

using namespace std;

class Integer;

class List : public Variable {
	public:
		List(const vector<any>& elements);
		virtual List& SetContext(Context* context) override;
		virtual List& SetPostion(Position start, Position end) override;
		vector<any> GetElements() const;
		any GetElement(const Integer& index) const;
		void push(const any& node);
		void pop();
		friend ostream& operator<<(ostream& lhs, const List& rhs);
	private:
		vector<any> elements;
};

