#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class ArrayAccess final :
    public Base,
    public FieldArrayNodeMixin<0>,
    public FieldIndexNodeMixin<1>
{
public:
    ArrayAccess(size_t ip, const Pex::StringTable::Index& result, BasePtr object, BasePtr index) :
        Base(2, ip, 0, result),
        FieldArrayNodeMixin(this, object),
        FieldIndexNodeMixin(this, index)
    {
    }
    virtual ~ArrayAccess() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
