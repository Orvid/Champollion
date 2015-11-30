#include "Declare.hpp"

#include <cassert>
#include "Visitor.hpp"


Node::Declare::Declare(size_t ip, BasePtr identifier, const Pex::StringTable::Index &type) :
    Base(1, ip, 0),
    FieldObjectNodeMixin(this, identifier),
    m_Type(type)
{
}

Node::Declare::~Declare()
{
}

std::shared_ptr<Node::Declare> Node::Declare::make(size_t ip, BasePtr identifier, const Pex::StringTable::Index &type)
{
    return std::shared_ptr<Declare>(new Declare(ip, identifier, type));
}

void Node::Declare::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const Pex::StringTable::Index &Node::Declare::getType() const
{
    return m_Type;
}
