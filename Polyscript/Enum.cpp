#include "Enum.h"
#include "EnumValue.h"
#include "RuntimeError.h"

using namespace std;


Enum::Enum(const vector<Token>& keys, string name): name(name)
{
	for (auto key : keys) {
		this->keys.push_back(key.GetValue().value());
	}
}
Enum& Enum::SetContext(Context* context)
{
	this->context = context;
	return *this;
}
Enum& Enum::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}
EnumValue Enum::get(string key) const
{
	if (!hasKey(key)) throw RuntimeError(key + " Is not defined in Enum " + name, context, this->start, this->end);
	size_t index = 0;
	while (index < this->keys.size()) {
		if (this->keys.at(index) == key) {
			return EnumValue(this->name,this->keys.at(index), index).SetContext(this->context).SetPostion(this->start,this->end);
		}
		index++;
	}

	throw RuntimeError(key + " Is not defined in Enum " + name, context, this->start, this->end);
}

bool Enum::empty() const
{
	return this->keys.size() == 0;
}

bool Enum::hasKey(string key) const
{
	if (empty()) return false;

	size_t index = 0;
	while(index < this->keys.size()){
		if (this->keys.at(index) == key) return true;
		index++;
	}

	return false;
}

size_t Enum::len() const
{
	return this->keys.size();
}

ostream& operator<<(ostream& lhs, const Enum& rhs)
{
	lhs << "<\x1B[36menum " << rhs.name << "\033[0m>";
	return lhs;
}

