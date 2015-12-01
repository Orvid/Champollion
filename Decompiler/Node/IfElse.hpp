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
        FieldElseNodeMixin(this, elseBody ? elseBody : Scope::make()),
        FieldElseIfNodeMixin(this, Scope::make())
    {
    }
    virtual ~IfElse() = default;

    static std::shared_ptr<IfElse> make(size_t ip, BasePtr condition, BasePtr body, BasePtr elseBody)
    {
        return std::make_shared<IfElse>(ip, condition, body, elseBody);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }

    virtual void computeInstructionBounds()
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
