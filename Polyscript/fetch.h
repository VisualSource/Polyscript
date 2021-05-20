#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <string>


class Context;

namespace fetch {
	httplib::Result Get(std::string url, Context* context);
}