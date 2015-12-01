#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class ArrayCreate final :
    public Base,
    public FieldIndexNodeMixin<0>
{
public:
    ArrayCreate(size_t ip, const Pex::StringTable::Index& result, BasePtr type, BasePtr size) :
        Base(1, ip, 0, result),
        m_Type(type),
        FieldIndexNodeMixin(this, size)
    {
    }
    virtual ~ArrayCreate() = default;

    static std::shared_ptr<ArrayCreate> make(size_t ip, const Pex::StringTable::Index& result, BasePtr type, BasePtr size)
    {
        return std::make_shared<ArrayCreate>(ip, result, type, size);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }

    const BasePtr getType() const { return m_Type; }

private:
    const BasePtr m_Type;
};

}
