#include <vip/utils/load_file.hpp>
#include <fstream>

namespace vip
{
    namespace utils
    {

        bool load_file(std::string source, std::string &content)
        {
            std::ifstream file(source);

            if (!file.is_open())
                return false;

            std::string line;

            while (std::getline(file, line))
            {
                content += line;
            }

            file.close();

            return true;
        }

    }
}
