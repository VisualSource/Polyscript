#include "SymbolTable.h"
#include "RuntimeError.h"
#include "Context.h"

using namespace ScopeTypes;

struct ScopeTypes::Node {
	string key;
	ScopeTypes::Var value;
	Node* next;
};

static ScopeTypes::NodePtr createNode(string key, Var value, NodePtr p){
	
	NodePtr n = new Node;

	n->key = key;
	n->value = value;
	n->next = p;

	return n;
}

SymbolTable::SymbolTable(){
	this->head = createNode(HEAD_OF_LIST, monostate {}, nullptr);
}

SymbolTable::SymbolTable(string secKey, Var value) {
	this->head = createNode(HEAD_OF_LIST, monostate{}, nullptr);
	this->insert(secKey, value);
}

SymbolTable::SymbolTable(const SymbolTable& lhs)
{
	NodePtr npp = lhs.head->next;
	this->head = createNode(HEAD_OF_LIST, monostate{}, nullptr);
	while (npp != nullptr) {
		this->insert(npp->key,npp->value);
		npp = npp->next;
	}
}

SymbolTable::~SymbolTable()
{
	NodePtr npp = head;
	NodePtr npc = nullptr;

	while (npp != nullptr){
		npc = npp;
		npp = npp->next;
		delete npc;
	}
}

bool SymbolTable::isEmpty() const
{
	return this->head->next == nullptr;
}

bool SymbolTable::hasVar(string key) const
{
	if (isEmpty()) return false;

	NodePtr head = this->head->next;
	bool foundKey = false;

	while (head != nullptr && !foundKey) {
		if (head->key == key) {
			foundKey = true;
			break;
		}
		head = head->next;
	}

	return foundKey;
}

Var SymbolTable::get(string key, Context* context) const
{
	if (hasVar(key)) {
		NodePtr npp = head;
		NodePtr npc = head->next;
		while (npc != nullptr) {
			if (npc->key == key) {
				return npc->value;
			}
			npp = npc;
			npc = npc->next;
		}
	}
	if (parent != nullptr) {
		return parent->get(key,context);
	}

	throw RuntimeError(key + " is undefined",context, context->GetPostion(),Position());
	
}

void SymbolTable::set(string key, Var value, Context* context)
{
	if (!hasVar(key)) throw RuntimeError(key + " is undefined",context,context->GetPostion(),context->GetPostion());
	NodePtr npp = head;
	NodePtr npc = head->next;
	while (npc != nullptr) {
		if (npc->key == key) {
			npc->value = value;
			return;
		}
		npp = npc;
		npc = npc->next;
	}
	if (parent != nullptr) {
		parent->set(key,value,context);
	}
}

void SymbolTable::add(string key, Var value, Context* context)
{
	if (hasVar(key)) {
		throw RuntimeError(key + " is already defined",context,context->GetPostion(),context->GetPostion());
	}
	this->insert(key, value);
}

void SymbolTable::setParent(SymbolTable* parent)
{
	this->parent = parent;
}

void SymbolTable::insert(string key, Var value)
{
	NodePtr npp = head;
	NodePtr npc = head->next;

	while (npc != nullptr) {
		if (npc->key == key) {
			throw runtime_error("var with name '" + key + "' already exists");
		}
		npp = npc;
		npc = npc->next;
	}
	npp->next = createNode(key, value, npc);

}

void SymbolTable::remove(string key)
{
	NodePtr npp = head;
	NodePtr npc = head->next;

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

bool ScopeTypes::isInteger(const Var& value)
{
	auto* v = std::get_if<Integer>(&value);
	return v != nullptr;
}

bool ScopeTypes::isFloat(const Var& value)
{
	auto* v = std::get_if<Float>(&value);
	return v != nullptr;
}

bool ScopeTypes::isString(const Var& value)
{
	auto* v = std::get_if<String>(&value);
	return v != nullptr;
}

bool ScopeTypes::isList(const Var& value)
{
	auto* v = std::get_if<List>(&value);
	return v != nullptr;
}

bool ScopeTypes::isBuiltIn(const Var& value)
{
	auto* v = std::get_if<BuiltInFunction>(&value);
	return v != nullptr;
}

bool ScopeTypes::isEnum(const Var& value)
{
	auto* v = std::get_if<Enum>(&value);
	return v != nullptr;
}

bool ScopeTypes::isFunction(const Var& value)
{
	auto* v = std::get_if<Function>(&value);
	return v != nullptr;
}
