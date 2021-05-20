#include "fetch.h"
#include "Context.h"
#include "RuntimeError.h"
#include <ostream>

using namespace std;


struct UrlObject {
	string orgin;
	string path;
};

static UrlObject SplitUrl(string url){
	size_t schemaLen = 7;

	if (url.find("https://", 0, 8) != string::npos) {
		schemaLen = 8;
	}

	size_t startPath = url.find("/",schemaLen);

	string path = url.substr(startPath);


	string orgin = url.substr(0,(url.length() - path.length()));


	return UrlObject {
		orgin,
		path
	};
}


httplib::Result fetch::Get(string url, Context* context){
	UrlObject object = SplitUrl(url);
	httplib::Client cli(object.orgin.c_str());

		httplib::Result res = cli.Get(object.path.c_str());

		if (res.error() != httplib::Error::Success) {
			throw RuntimeError("Fetch request error: " + std::to_string((int)res.error()), context, context->GetPostion(), context->GetPostion());
		}

		return res;
}