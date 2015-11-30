#include "UnaryOperator.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::UnaryOperator::UnaryOperator(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index &result, const std::string &op, Node::BasePtr value) :
    Base(1, ip, precedence, result),
    FieldValueNodeMixin(this, value),
    m_Op(op)
{
}

Node::UnaryOperator::~UnaryOperator()
{
}

std::shared_ptr<Node::UnaryOperator> Node::UnaryOperator::make(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index &result, const std::string &op, Node::BasePtr value)
{
    return std::shared_ptr<Node::UnaryOperator>(new UnaryOperator(ip, precedence, result, op, value));
}

void Node::UnaryOperator::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const std::string &Node::UnaryOperator::getOperator() const
{
    return m_Op;
}
