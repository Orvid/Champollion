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

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
