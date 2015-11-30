#include "While.hpp"

#include <cassert>
#include "Visitor.hpp"


Node::While::While(size_t ip, Node::BasePtr condition, Node::BasePtr body) :
    Base(2, ip, 10),
    FieldConditionNodeMixin(this, condition),
    FieldBodyNodeMixin(this, body)
{
}

Node::While::~While()
{
}

std::shared_ptr<Node::While> Node::While::make(size_t ip, Node::BasePtr condition, Node::BasePtr body)
{
    return std::shared_ptr<While>(new While(ip, condition, body));
}

void Node::While::visit(Node::Visitor *visitor)
{
    visitor->visit(this);
}

void Node::While::computeInstructionBounds()
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
