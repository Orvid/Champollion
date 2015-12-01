#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

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

    static std::shared_ptr<Cast> make(size_t ip, const Pex::StringTable::Index& result, BasePtr value, const Pex::StringTable::Index& type)
    {
        return std::make_shared<Cast>(ip, result, value, type);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }

    const Pex::StringTable::Index& getType() { return m_Type; }

private:
    Pex::StringTable::Index m_Type;
};

}
