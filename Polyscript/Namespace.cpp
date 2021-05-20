#include "Namespace.h"
#include "Context.h"

using namespace std;

Namespace::Namespace(string name,Context* ctx): nsctx(ctx), name(name)
{
}

Namespace::~Namespace()
{
	//delete nsctx->GetScope();
	//delete nsctx;
}

Namespace& Namespace::SetContext(Context* context)
{
	this->context = context;
	return *this;
}

Namespace& Namespace::SetPostion(Position start, Position end)
{
	this->start = start;
	this->end = end;
	return *this;
}

Context* Namespace::GetNamespaceCtx() const
{
	return nsctx;
}

string Namespace::GetName() const
{
	return name;
}

std::ostream& operator<<(std::ostream& lhs, const Namespace& other)
{
	lhs << "<\x1B[92mnamespace " << other.name << "\033[0m>";
	return lhs;
}
