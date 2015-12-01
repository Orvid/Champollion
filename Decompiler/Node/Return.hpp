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

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
