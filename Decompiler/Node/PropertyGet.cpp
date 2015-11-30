#include "PropertyGet.hpp"

#include <cassert>
#include "Visitor.hpp"
/*
Node::PropertyGet::PropertyGet(size_t ip, const Papyrus::StringTable::Index &result, Node::BasePtr object, const Papyrus::StringTable::Index &property) :
    Base(1, ip, 0, result),
    PropertyMixin(property),
    FieldObjectNodeMixin(this, object)
{
}

Node::PropertyGet::~PropertyGet()
{
}

std::shared_ptr<Node::PropertyGet> Node::PropertyGet::make(size_t ip, const Papyrus::StringTable::Index &result, Node::BasePtr object, const Papyrus::StringTable::Index &property)
{
    return std::shared_ptr<PropertyGet>(new PropertyGet(ip, result, object, property));
}

void Node::PropertyGet::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}
*/
