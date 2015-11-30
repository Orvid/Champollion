#include "AssignOperator.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::AssignOperator::AssignOperator(size_t ip, Node::BasePtr destination, const std::string &op, Node::BasePtr expr):
    Base(2, ip, 10),
    FieldValueNodeMixin(this, expr),
    FieldDestinationNodeMixin(this, destination),
    m_Operator(op)
{
}

Node::AssignOperator::~AssignOperator()
{
}

std::shared_ptr<Node::AssignOperator> Node::AssignOperator::make(size_t ip, Node::BasePtr destination, const std::string &op, Node::BasePtr expr)
{
    return std::shared_ptr<AssignOperator>(new AssignOperator(ip, destination, op, expr));
}

void Node::AssignOperator::visit(Node::Visitor *visitor)
{
    return visitor->visit(this);
}

const std::string &Node::AssignOperator::getOperator() const
{
    return m_Operator;
}
