#pragma once

#include <cassert>
#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class While final :
    public Base,
    public FieldConditionNodeMixin<0>,
    public FieldBodyNodeMixin<1>
{
public:
    While(size_t ip, BasePtr condition, BasePtr body) :
        Base(2, ip, 10),
        FieldConditionNodeMixin(this, condition),
        FieldBodyNodeMixin(this, body)
    {
    }
    virtual ~While() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    void computeInstructionBounds() override
    {
        Base::computeInstructionBounds();
        if (getCondition()->getBegin() == -1 && getCondition()->getEnd() == -1 && getBody()->size() != 0)
        {
            m_Begin = getBody()->getBegin() - 1;
            m_End = m_Begin;
        }
        else
        {
            m_Begin = getCondition()->getBegin();
            m_End = getCondition()->getEnd() + 1;
        }
    }
};

}
