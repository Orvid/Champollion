#include "Copy.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::Copy::Copy(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr value) :
    Base(1, ip, 10, result),
    FieldValueNodeMixin(this, value)
{
}

Node::Copy::~Copy()
{
}

std::shared_ptr<Node::Copy> Node::Copy::make(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr value)
{
    return std::shared_ptr<Node::Copy>(new Node::Copy(ip, result, value));
}

void Node::Copy::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}
