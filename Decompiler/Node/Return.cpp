#include "Return.hpp"

#include <cassert>
#include "Visitor.hpp"


Node::Return::Return(size_t ip, Node::BasePtr expr) :
    Base(1, ip, 10),
    FieldValueNodeMixin(this, expr)
{
}

Node::Return::~Return()
{
}

std::shared_ptr<Node::Return> Node::Return::make(size_t ip, Node::BasePtr expr)
{
    return std::shared_ptr<Return>(new Return(ip, expr));
}

void Node::Return::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}
