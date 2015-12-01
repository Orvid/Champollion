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

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
