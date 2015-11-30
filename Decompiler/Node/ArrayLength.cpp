#include "ArrayLength.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::ArrayLength::ArrayLength(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr object) :
    Base(1, ip, 0, result),
    FieldArrayNodeMixin(this, object)
{
}

Node::ArrayLength::~ArrayLength()
{
}

std::shared_ptr<Node::ArrayLength> Node::ArrayLength::make(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr object)
{
    return std::shared_ptr<ArrayLength>(new ArrayLength(ip, result, object));
}

void Node::ArrayLength::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}
