#include "Context.h"
#include "SymbolTable.h"

Context::Context(std::string display_name, SymbolTable* scope, Context* parent, Position parent_entry): display_name(display_name), scope(scope), parent(parent), parent_entry(parent_entry)
{
}
