#include "Cast.hpp"

#include <cassert>
#include "Visitor.hpp"


Node::Cast::Cast(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr value, const Pex::StringTable::Index &type) :
    Base(1, ip, 1, result),
    FieldValueNodeMixin(this, value),
    m_Type(type)
{
}

Node::Cast::~Cast()
{
}

std::shared_ptr<Node::Cast> Node::Cast::make(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr value, const Pex::StringTable::Index &type)
{
    return std::shared_ptr<Node::Cast>(new Node::Cast(ip, result, value, type));
}

void Node::Cast::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const Pex::StringTable::Index &Node::Cast::getType()
{
    return m_Type;
}
