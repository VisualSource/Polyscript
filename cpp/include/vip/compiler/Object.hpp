#pragma once
#include <iostream>
#include <cstdint>
namespace compiler
{
    class Object
    {
    private:
        uint kind = 0;

    public:
        Object(uint kind) : kind(kind) {}
        virtual ~Object() = default;
        inline uint getKind() const { return kind; }
        virtual void print(std::ostream &where) const { where << "object"; };
        inline friend std::ostream &operator<<(std::ostream &out, const Object &f)
        {
            f.print(out);
            return out;
        }
    };

}