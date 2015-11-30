#include "IfElse.hpp"

#include <cassert>
#include "Visitor.hpp"

#include "Scope.hpp"

Node::IfElse::IfElse(size_t ip, Node::BasePtr condition, Node::BasePtr body, Node::BasePtr elseBody) :
    Base(4, ip, 10),
    FieldConditionNodeMixin(this, condition),
    FieldBodyNodeMixin(this, body),
    FieldElseNodeMixin(this, (elseBody)?elseBody:Scope::make()),
    FieldElseIfNodeMixin(this, Scope::make())
{
}

Node::IfElse::~IfElse()
{
}

std::shared_ptr<Node::IfElse> Node::IfElse::make(size_t ip, Node::BasePtr condition, Node::BasePtr body, Node::BasePtr elseBody)
{
    return std::shared_ptr<IfElse>(new IfElse(ip, condition, body, elseBody));
}

void Node::IfElse::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

void Node::IfElse::computeInstructionBounds()
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
