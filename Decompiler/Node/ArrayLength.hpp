#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class ArrayLength final :
    public Base,
    public FieldArrayNodeMixin<0>
{
public:
    ArrayLength(size_t ip, const Pex::StringTable::Index& result, BasePtr object) :
        Base(1, ip, 0, result),
        FieldArrayNodeMixin(this, object)
    {
    }
    virtual ~ArrayLength() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
