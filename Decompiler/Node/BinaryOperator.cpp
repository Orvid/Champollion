#include "BinaryOperator.hpp"

#include <cassert>

#include "Visitor.hpp"

Node::BinaryOperator::BinaryOperator(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index &result, Node::BasePtr left, const std::string &op, Node::BasePtr right) :
    Base(2, ip, precedence, result),
    FieldLeftNodeMixin(this, left),
    FieldRightNodeMixin(this, right),
    m_Op(op)
{
}

Node::BinaryOperator::~BinaryOperator()
{
}

std::shared_ptr<Node::BinaryOperator> Node::BinaryOperator::make(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index &result, Node::BasePtr left, const std::string &op, Node::BasePtr right)
{
    return std::shared_ptr<Node::BinaryOperator>(new BinaryOperator(ip, precedence, result, left, op, right));
}

void Node::BinaryOperator::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const std::string& Node::BinaryOperator::getOperator() const
{
    return m_Op;
}

void Node::BinaryOperator::computeInstructionBounds()
{
    Base::computeInstructionBounds();
}

