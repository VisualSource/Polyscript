#pragma once
#include <string>
#include <vector>

namespace compile
{
    void compileCil(std::vector<std::string> files, std::vector<std::string> flags, std::string outdir);

} // namespace compile
