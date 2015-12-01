#pragma once

#include <cassert>
#include <memory>

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

class Params final : public Base
{
public:
    Params() : Base(0, -1, 10) { }
    virtual ~Params() = default;

    static std::shared_ptr<Params> make()
    {
        return std::make_shared<Params>();
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }
};

}
