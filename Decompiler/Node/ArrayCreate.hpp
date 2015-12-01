#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class ArrayCreate final :
    public Base,
    public FieldIndexNodeMixin<0>
{
public:
    ArrayCreate(size_t ip, const Pex::StringTable::Index& result, const Pex::StringTable::Index& type, BasePtr size) :
        Base(1, ip, 0, result),
        m_Type(type),
        FieldIndexNodeMixin(this, size)
    {
    }
    virtual ~ArrayCreate() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    const Pex::StringTable::Index& getType() const { return m_Type; }

private:
    const Pex::StringTable::Index& m_Type;
};

}
