#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Scope.hpp"
#include "Visitor.hpp"

namespace Node {

class IfElse final :
    public Base,
    public FieldConditionNodeMixin<0>,
    public FieldBodyNodeMixin<1>,
    public FieldElseNodeMixin<2>,
    public FieldElseIfNodeMixin<3>
{
public:
    IfElse(size_t ip, BasePtr condition, BasePtr body, BasePtr elseBody) :
        Base(4, ip, 10),
        FieldConditionNodeMixin(this, condition),
        FieldBodyNodeMixin(this, body),
        FieldElseNodeMixin(this, elseBody ? elseBody : std::make_shared<Scope>()),
        FieldElseIfNodeMixin(this, std::make_shared<Scope>())
    {
    }
    virtual ~IfElse() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    void computeInstructionBounds() override
    {
        Base::computeInstructionBounds();
        if (getCondition()->getBegin() == -1)
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
