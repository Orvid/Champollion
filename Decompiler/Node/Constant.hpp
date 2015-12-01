#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "Visitor.hpp"
#include "Pex/Value.hpp"

namespace Node {

class Constant final : public Base
{
public:
    Constant(size_t ip, const Pex::Value& constant) :
        Base(0, ip, 0),
        m_Constant(constant)
    {
    }
    virtual ~Constant() = default;

    static std::shared_ptr<Constant> make(size_t ip, const Pex::Value& constant)
    {
        return std::make_shared<Constant>(ip, constant);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }

    const Pex::Value& getConstant() const { return m_Constant; }

private:
    Pex::Value m_Constant;
};

}
