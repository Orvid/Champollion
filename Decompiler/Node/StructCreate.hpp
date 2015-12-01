#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

class StructCreate final : public Base
{
public:
    StructCreate(size_t ip, const Pex::StringTable::Index& result, const Pex::StringTable::Index& type) :
        Base(0, ip, 0, result),
        m_Type(type)
    {
    }
    virtual ~StructCreate() = default;

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
