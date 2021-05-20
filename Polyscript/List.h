#pragma once
#include "Variable.h"
#include <vector>
#include <any>

class Integer;

class List : public Variable {
	public:
		List(const std::vector<std::any>& elements);
		virtual List& SetContext(Context* context) override;
		virtual List& SetPostion(Position start, Position end) override;
		std::vector<std::any> GetElements() const;
		std::any GetElement(const Integer& index) const;
		void push(const std::any& node);
		void pop();
		friend std::ostream& operator<<(std::ostream& lhs, const List& rhs);
	private:
		std::vector<std::any> elements;
};

