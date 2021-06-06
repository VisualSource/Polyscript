#include "SymbolTable.h"
#include "RuntimeError.h"
#include "Context.h"

using namespace std;
using namespace ScopeTypes;

static ScopeTypes::NodePtr createNode(string key, Var value, NodePtr p, bool writable = true) {
	
	NodePtr n = new ScopeTypes::Node;

	n->writable = writable;
	n->key = key;
	n->value = value;
	n->next = p;

	return n;
}

SymbolTable::SymbolTable(){
	this->head = createNode(HEAD_OF_LIST, Null(), nullptr);
}

SymbolTable::SymbolTable(string secKey, Var value) {
	this->head = createNode(HEAD_OF_LIST, Null(), nullptr);
	this->insert(secKey, value);
}

SymbolTable::SymbolTable(const SymbolTable& lhs)
{
	NodePtr npp = lhs.head->next;
	this->head = createNode(HEAD_OF_LIST, Null(), nullptr);
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

Var SymbolTable::set(string key, Var value, Context* context)
{
	if (!hasVar(key)) throw RuntimeError(key + " is undefined",context,context->GetPostion(),context->GetPostion());
	NodePtr npp = head;
	NodePtr npc = head->next;
	while (npc != nullptr) {
		if (npc->key == key) {
			if(!npc->writable) throw RuntimeError(key + " is a const", context, context->GetPostion(), context->GetPostion());
			npc->value = value;
			return npc->value;
		}
		npp = npc;
		npc = npc->next;
	}
	if (parent != nullptr) {
		return parent->set(key,value,context);
	}

	throw RuntimeError("Failed to set variable", context, context->GetPostion(), context->GetPostion());
}

void SymbolTable::add(string key, Var value, Context* context, bool writable)
{
	if (hasVar(key)) {
		throw RuntimeError(key + " is already defined",context,context->GetPostion(),context->GetPostion());
	}
	this->insert(key, value, writable);
}

void SymbolTable::setParent(SymbolTable* parent)
{
	this->parent = parent;
}

void SymbolTable::insert(string key, Var value, bool writable)
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
	npp->next = createNode(key, value, npc, writable);

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

