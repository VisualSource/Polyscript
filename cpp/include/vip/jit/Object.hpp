#pragma once
#include <iostream>

namespace jit
{
    class Object
    {
    private:
        unsigned int kind = 0;

    public:
        Object(unsigned int kind) : kind(kind) {}
        virtual ~Object() = default;
        inline unsigned int getKind() const { return kind; }
        virtual void print(std::ostream &where) const { where << "object"; };
        inline friend std::ostream &operator<<(std::ostream &out, const Object &f)
        {
            f.print(out);
            return out;
        }
    };
}