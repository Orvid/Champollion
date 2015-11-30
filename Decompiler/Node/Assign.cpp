#include "Assign.hpp"

#include <cassert>
#include "Visitor.hpp"


Node::Assign::Assign(size_t ip, Node::BasePtr destination, Node::BasePtr value) :
    Base(2, ip, 10),
    FieldValueNodeMixin(this, value),
    FieldDestinationNodeMixin(this, destination)
{
}

Node::Assign::~Assign()
{
}

std::shared_ptr<Node::Assign> Node::Assign::make(size_t ip, Node::BasePtr destination, Node::BasePtr value)
{
    return std::shared_ptr<Assign>(new Assign(ip, destination, value));
}

void Node::Assign::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}
