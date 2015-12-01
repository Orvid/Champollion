#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class Return final :
    public Base,
    public FieldValueNodeMixin<0>
{
public:
    Return(size_t ip, BasePtr expr) :
        Base(1, ip, 10),
        FieldValueNodeMixin(this, expr)
    {
    }
    virtual ~Return() = default;

    static std::shared_ptr<Return> make(size_t ip, BasePtr expr)
    {
        return std::make_shared<Return>(ip, expr);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
