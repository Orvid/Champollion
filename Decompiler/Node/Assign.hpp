#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class Assign final :
    public Base,
    public FieldValueNodeMixin<0>,
    public FieldDestinationNodeMixin<1>
{
public:
    Assign(size_t ip, BasePtr destination, BasePtr value) :
        Base(2, ip, 10),
        FieldValueNodeMixin(this, value),
        FieldDestinationNodeMixin(this, destination)
    {
    }
    virtual ~Assign() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
