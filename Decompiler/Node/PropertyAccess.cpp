#include "PropertyAccess.hpp"

#include <cassert>
#include "Visitor.hpp"


Node::PropertyAccess::PropertyAccess(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr object, const Pex::StringTable::Index &property) :
    Base(1, ip, 0, result),
    FieldObjectNodeMixin(this, object),
    m_Property(property)
{
}

Node::PropertyAccess::~PropertyAccess()
{
}

std::shared_ptr<Node::PropertyAccess> Node::PropertyAccess::make(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr object, const Pex::StringTable::Index &property)
{
    return std::shared_ptr<PropertyAccess>(new PropertyAccess(ip, result, object, property));
}

void Node::PropertyAccess::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const Pex::StringTable::Index &Node::PropertyAccess::getProperty() const
{
    return m_Property;
}
