#pragma once
#include <string>

namespace ast
{

    class Node
    {
    private:
        unsigned int start;
        unsigned int end;
        unsigned int kind;

    public:
        Node(unsigned int start, unsigned int end, unsigned int kind) : start(start), end(end), kind(kind) {}
        virtual ~Node() = default;
        /// @brief Get starting position of node from source
        /// @return
        inline unsigned int getStart() { return start; }
        /// @brief Get ending position of node from source
        /// @return
        inline unsigned int getEnd() { return end; }
        /// @brief Get the kind of this node.
        /// @return
        inline unsigned int getKind() { return kind; }
        /// @brief get string rep of node
        /// @param padding
        /// @return
        virtual std::string toString(int padding = 0);
    };

}