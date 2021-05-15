#include "List.h"
#include "Integer.h"
#include "InterpreterUtils.h"
#include "RuntimeError.h"

List::List(const vector<any>& elements): elements(elements)
{
}

List& List::SetContext(Context* context)
{
	this->context = context;
	return *this;
}

List& List::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}

vector<any> List::GetElements() const
{
	return elements;
}

any List::GetElement(const Integer& index) const
{
	try
	{
		return this->elements.at(index.GetValue());
	}
	catch (const std::exception&)
	{
		throw RuntimeError("Out of range",this->context,this->start,this->end);
	}
	
}

void List::push(const any& node)
{
	this->elements.push_back(node);
}

void List::pop()
{
	this->elements.pop_back();
}

ostream& operator<<(ostream& lhs, const List& rhs)
{
	lhs << "[";
	for (auto el : rhs.elements) {
		InterTypes::print(lhs, el);
		lhs << ", ";
	}
	lhs << "]";
	return lhs;
}
