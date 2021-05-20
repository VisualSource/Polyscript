#include "RuntimeError.h"


RuntimeError::RuntimeError(std::string details, Context* context, Position start, Position end): PolyscriptError("Runtime Error",details, start, end), context(context) {
}

std::ostream& operator<<(std::ostream& lhs, const RuntimeError& error) {
	lhs << "Traceback (most recent call last):" << std::endl;
	
	Context* ctx = error.context;
	Position pos = error.GetStart();
	while (ctx != nullptr) {
		lhs << "  File " << pos.GetFn() << ", line " << (pos.GetLn() + 1) <<  ", in " + ctx->GetDisplayName() << std::endl;

		pos = ctx->GetPostion();
		ctx = ctx->GetContextParent();
	}
	lhs << static_cast<const PolyscriptError&>(error);
	return lhs;
}
