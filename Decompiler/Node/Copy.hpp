#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class Copy final :
    public Base,
    public FieldValueNodeMixin<0>
{
public:
    Copy(size_t ip, const Pex::StringTable::Index& result, BasePtr value) :
        Base(1, ip, 10, result),
        FieldValueNodeMixin(this, value)
    {
    }
    virtual ~Copy() = default;

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
