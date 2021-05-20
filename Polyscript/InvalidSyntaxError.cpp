#include "InvalidSyntaxError.h"

InvalidSyntaxError::InvalidSyntaxError(std::string details, Position start, Position end): PolyscriptError("Invalid Syntax", details,start,end) {
}
