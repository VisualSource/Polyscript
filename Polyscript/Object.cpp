#include "Object.h"
#include "Integer.h"
#include "Null.h"
#include "RuntimeError.h"
#include "InterpreterUtils.h"

using namespace std;


static Property* createNode(string key, any value, Property* p) {

	Property* n = new Property;

	n->key = key;
	n->value = value;
	n->next = p;

	return n;
}

Object::Object(const vector<ObjectProperties>& values, Context* ctx)
{
	this->head = createNode(OBJECT_HEAD_OF_LIST, Null(), nullptr);
	this->SetContext(ctx);
	for (auto item: values) {
		this->insert(item.name, item.expr);
	}
}

Object::Object(const Object& lhs)
{
	Property* npp = lhs.head->next;
	this->head = createNode(OBJECT_HEAD_OF_LIST, Null(), nullptr);
	while (npp != nullptr) {
		this->insert(npp->key, npp->value);
		npp = npp->next;
	}
}

Object::~Object()
{
	Property* npp = head;
	Property* npc = nullptr;

	while (npp != nullptr) {
		npc = npp;
		npp = npp->next;
		delete npc;
	}
}

Object& Object::SetContext(Context* context)
{
	this->context = context;
	return *this;
}

Object& Object::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}

bool Object::empty() const
{
	return this->head->next == nullptr;
}

any Object::GetValue(std::string key)
{
	if (HasProperty(key)) {
		Property* npp = head;
		Property* npc = head->next;
		while (npc != nullptr) {
			if (npc->key == key) {
				return npc->value;
			}
			npp = npc;
			npc = npc->next;
		}
	}
	throw RuntimeError(key + " is undefined", context, start, end);
}

void Object::SetValue(std::string key, any value)
{
	if (!HasProperty(key)) throw RuntimeError(key + " is undefined", context, start, end);
	Property* npp = head;
	Property* npc = head->next;
	while (npc != nullptr) {
		if (npc->key == key) {
			npc->value = value;
			return;
		}
		npp = npc;
		npc = npc->next;
	}
}

Integer Object::HasProperty(std::string key)
{
	if (empty()) return Integer::False().SetContext(context);

	Property* head = this->head->next;
	bool foundKey = false;

	while (head != nullptr && !foundKey) {
		if (head->key == key) {
			foundKey = true;
			break;
		}
		head = head->next;
	}

	return Integer(foundKey).PrintBool(true).SetContext(this->context);
}

Integer Object::operator==(const Object& rhs) const
{
	return Integer::False().SetContext(this->context);
}

Integer Object::operator!=(const Object& rhs) const
{
	return Integer::True().SetContext(this->context);
}

void Object::insert(std::string key, any value)
{
	Property* npp = head;
	Property* npc = head->next;

	while (npc != nullptr) {
		if (npc->key == key) {
			throw RuntimeError("property with name '" + key + "' already exists",context,start,end);
		}
		npp = npc;
		npc = npc->next;
	}
	npp->next = createNode(key, value, npc);
}

void Object::remove(std::string key)
{
	Property* npp = head;
	Property* npc = head->next;

	while (npc != nullptr) {
		if (npc->key == key) {
			npp->next = npc->next;
			delete npc;
			break;
		}
		npp = npc;
		npc = npc->next;
	}
}

std::ostream& operator<<(std::ostream& lhs, const Object& other)
{
	lhs << "{";

	Property* npp = other.head;
	Property* npc = other.head->next;

	while (npc != nullptr) {
		lhs << "\n  " << npc->key << ": ";
		InterTypes::print(lhs, npc->value);
		lhs << ",";
		npp = npc;
		npc = npc->next;
	}

	lhs << "\n}";
	return lhs;
}
