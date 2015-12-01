#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class PropertyAccess final :
    public Base,
    public FieldObjectNodeMixin<0>
{
public:
    PropertyAccess(size_t ip, const Pex::StringTable::Index& result, BasePtr object, const Pex::StringTable::Index& property) :
        Base(1, ip, 0, result),
        FieldObjectNodeMixin(this, object),
        m_Property(property)
    {
    }
    virtual ~PropertyAccess() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    const Pex::StringTable::Index& getProperty() const { return m_Property; }

private:
    Pex::StringTable::Index m_Property;
};

}
