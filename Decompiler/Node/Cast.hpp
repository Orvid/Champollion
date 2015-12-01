#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class Cast final :
    public Base,
    public FieldValueNodeMixin<0>
{
public:
    Cast(size_t ip, const Pex::StringTable::Index& result, BasePtr value, const Pex::StringTable::Index& type) :
        Base(1, ip, 1, result),
        FieldValueNodeMixin(this, value),
        m_Type(type)
    {
    }
    virtual ~Cast() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    const Pex::StringTable::Index& getType() { return m_Type; }

private:
    Pex::StringTable::Index m_Type;
};

}
