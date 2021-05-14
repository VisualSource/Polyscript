#include "IllegalCharError.h"

IllegalCharError::IllegalCharError(string details, Position start, Position end): PolyscriptError("Illegal Character",details, start, end) {
}
