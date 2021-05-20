#include "IllegalCharError.h"


IllegalCharError::IllegalCharError(std::string details, Position start, Position end): PolyscriptError("Illegal Character",details, start, end) {
}
