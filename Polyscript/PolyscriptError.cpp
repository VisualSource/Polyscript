#include "PolyscriptError.h"

PolyscriptError::PolyscriptError() {
    details = name = "";
    end = start = Position();
}

PolyscriptError::PolyscriptError(string name, string details, Position start, Position end): name(name), details(details), start(start), end(end) {
}

string PolyscriptError::as_str(string ad)
{
    string error;
    error += name + "'" + details + ad + "' at Ln: " + to_string(start.GetLn() + 1) + " Col: " + to_string(start.GetCol()) + " in file: " + start.GetFn();
    return error;
}

std::ostream& operator<<(std::ostream& lhs, const PolyscriptError& err) {
    lhs << err.name << " '" << err.details << "' at Ln: " << err.start.GetLn() + 1 << " Col: " << err.start.GetCol() << " in file: " << err.start.GetFn();
    return lhs;
}
