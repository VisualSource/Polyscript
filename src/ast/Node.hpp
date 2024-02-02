#pragma once
#include <string>

namespace ast
{

    class Node
    {
    private:
        unsigned int start;
        unsigned int end;

    public:
        Node() : start(0), end(0) {}
        virtual ~Node() = default;
        unsigned int getStart() { return start; }
        unsigned int getEnd() { return end; }

        virtual std::string toString(int padding = 0);
    };

}