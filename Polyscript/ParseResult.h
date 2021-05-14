#pragma once
#include <any>
#include <optional>
#include "PolyscriptError.h"

class ParseResult {
	public:
		ParseResult();
		any Register(const any& res);
		ParseResult& success(const any& node);
		ParseResult& failure(const PolyscriptError& err);
		any GetNode() const;
		optional<PolyscriptError> GetErr() const;
		bool hasErr() const;
	private: 
		any node;
		optional<PolyscriptError> error;

};

