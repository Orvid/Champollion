#include "ArrayAccess.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::ArrayAccess::ArrayAccess(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr object, Node::BasePtr index) :
    Base(2, ip, 0, result),
    FieldArrayNodeMixin(this, object),
    FieldIndexNodeMixin(this, index)
{
}

Node::ArrayAccess::~ArrayAccess()
{
}

std::shared_ptr<Node::ArrayAccess> Node::ArrayAccess::make(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr object, Node::BasePtr index)
{
    return std::shared_ptr<ArrayAccess>(new ArrayAccess(ip, result, object, index));
}

void Node::ArrayAccess::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

