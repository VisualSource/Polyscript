#include "./compiler.hpp"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#define UNUSED(x) (void)(x)

namespace compile
{
        void compileCil(std::vector<std::string> files, std::vector<std::string> flags, std::string outfile)
        {
                std::cout << "Building..." << std::endl;
                UNUSED(flags);
                // https://stackoverflow.com/questions/9120596/run-another-program-in-linux-from-a-c-program
                std::string file = files.at(0);
// https://stackoverflow.com/questions/6649936/c-compiling-on-windows-and-linux-ifdef-switch
#ifdef __linux__
                std::string cmd = "ilasm -exe -output:" + outfile + " " + file;
                std::cout.flush();
                std::system(cmd.c_str()); // should be using something eles linux seems to be fopen and for windows createprocess
#elif _WIN32
                // if windows
                // try find ilasm
                // somewhere around here.
                // C:\Windows\Microsoft.NET\Framework64\v4.0.30319\ilasm SOURCE
#else
                throw std::runtime_error("Current system is unsupported.");
#endif
        }
}