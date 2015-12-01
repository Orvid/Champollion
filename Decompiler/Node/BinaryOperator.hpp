#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class BinaryOperator final :
    public Base,
    public FieldLeftNodeMixin<0>,
    public FieldRightNodeMixin<1>
{
public:
    BinaryOperator(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result, BasePtr left, const std::string& op, BasePtr right) :
        Base(2, ip, precedence, result),
        FieldLeftNodeMixin(this, left),
        FieldRightNodeMixin(this, right),
        m_Op(op)
    {
    }
    virtual ~BinaryOperator() = default;

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }

    virtual void computeInstructionBounds()
    {
        Base::computeInstructionBounds();
    }

    const std::string& getOperator() const { return m_Op; }

private:
    std::string m_Op;
};

}
