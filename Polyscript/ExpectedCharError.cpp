#include "ExpectedCharError.h"

ExpectedCharError::ExpectedCharError(string details, Position start, Position end): PolyscriptError("Expected Char Error", details,start,end)
{
}
