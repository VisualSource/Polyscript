#pragma once
#include "../Object.hpp"
#include "../Consts.hpp"

namespace jit
{
    class Null : public Object
    {
    public:
        Null() : Object(consts::ID_NULL) {}
        void print(std::ostream &where) const override { where << "null"; }
    };
} // namespace jit
