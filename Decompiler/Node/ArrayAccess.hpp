#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

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

    static std::shared_ptr<ArrayAccess> make(size_t ip, const Pex::StringTable::Index& result, BasePtr object, BasePtr index)
    {
        return std::make_shared<ArrayAccess>(ip, result, object, index);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
