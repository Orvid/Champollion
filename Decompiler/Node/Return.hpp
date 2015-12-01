#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class Return final :
    public Base,
    public FieldValueNodeMixin<0>
{
public:
    Return(size_t ip, BasePtr expr) :
        Base(1, ip, 10),
        FieldValueNodeMixin(this, expr)
    {
    }
    virtual ~Return() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
