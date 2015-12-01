#pragma once

#include <cassert>
#include <memory>

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

class Scope final : public Base
{
public:
    Scope() : Base(0, -1, 10) { }
    virtual ~Scope() = default;

    static std::shared_ptr<Scope> make()
    {
        return std::make_shared<Scope>();
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
