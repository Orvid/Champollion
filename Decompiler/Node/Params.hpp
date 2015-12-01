#pragma once

#include <cassert>

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

class Params final : public Base
{
public:
    Params() : Base(0, -1, 10) { }
    virtual ~Params() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
