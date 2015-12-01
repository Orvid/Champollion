#pragma once

#include <cassert>

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

class Scope final : public Base
{
public:
    Scope() : Base(0, -1, 10) { }
    virtual ~Scope() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
