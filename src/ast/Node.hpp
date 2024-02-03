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
        /// @brief Get starting position of node from source
        /// @return
        unsigned int getStart() { return start; }
        /// @brief Get ending position of node from source
        /// @return
        unsigned int getEnd() { return end; }

        /// @brief get string rep of node
        /// @param padding
        /// @return
        virtual std::string toString(int padding = 0);
    };

}