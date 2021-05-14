#include "RuntimeError.h"

RuntimeError::RuntimeError(string details, Context* context, Position start, Position end): PolyscriptError("Runtime Error",details, start, end), context(context) {
}

string RuntimeError::GenerateTraceback()
{
	return string();
}

std::ostream& operator<<(std::ostream& lhs, const RuntimeError& error) {
	lhs << "Traceback (most recent call last):" << endl;
	
	Context* ctx = error.context;
	Position pos = error.GetStart();
	while (ctx != nullptr) {
		lhs << "  File " << pos.GetFn() << ", line " << (pos.GetLn() + 1) <<  ", in " + ctx->GetDisplayName() << endl;

		pos = ctx->GetPostion();
		ctx = ctx->GetContextParent();
	}
	lhs << static_cast<const PolyscriptError&>(error);
	return lhs;
}
