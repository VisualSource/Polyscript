#include "ParseResult.h"

ParseResult::ParseResult() {
    error = nullopt;
}

any ParseResult::Register(const any& res) {
    if(res.type() == typeid(ParseResult)){
        ParseResult r = any_cast<ParseResult>(res);
        if (r.hasErr()) {
            error = r.GetErr();
        }
        return r.GetNode();
    }

    return res;
}

ParseResult& ParseResult::success(const any& node) {
    this->node = node;
    return *this;
}

ParseResult& ParseResult::failure(const PolyscriptError& err) {
    this->error = err;
    return *this;
}

any ParseResult::GetNode() const {
    return node;
}

optional<PolyscriptError> ParseResult::GetErr() const {
    return error;
}

bool ParseResult::hasErr() const {
    return error.has_value();
}
