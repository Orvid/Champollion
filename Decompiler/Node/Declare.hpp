#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class Declare final :
    public Base,
    public FieldObjectNodeMixin<0>
{
public:
    Declare(size_t ip, BasePtr identifier, const Pex::StringTable::Index& type) :
        Base(1, ip, 0),
        FieldObjectNodeMixin(this, identifier),
        m_Type(type)
    {
    }
    virtual ~Declare() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    const Pex::StringTable::Index& getType() const { return m_Type; }

private:
    Pex::StringTable::Index m_Type;
};

}
