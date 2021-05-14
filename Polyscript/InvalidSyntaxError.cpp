#include "InvalidSyntaxError.h"

InvalidSyntaxError::InvalidSyntaxError(string details, Position start, Position end): PolyscriptError("Invalid Syntax", details,start,end) {
}
