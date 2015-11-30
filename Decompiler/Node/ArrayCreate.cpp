#include "ArrayCreate.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::ArrayCreate::ArrayCreate(size_t ip, const Pex::StringTable::Index &result, const Node::BasePtr type, Node::BasePtr size) :
    Base(1, ip, 0, result),
    m_Type(type),
    FieldIndexNodeMixin(this, size)
{
}

Node::ArrayCreate::~ArrayCreate()
{
}

std::shared_ptr<Node::ArrayCreate> Node::ArrayCreate::make(size_t ip, const Pex::StringTable::Index &result, const Node::BasePtr type, Node::BasePtr size)
{
    assert(result.isValid());
    return std::shared_ptr<ArrayCreate>(new ArrayCreate(ip, result, type, size));
}

void Node::ArrayCreate::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const Node::BasePtr Node::ArrayCreate::getType() const
{
    return m_Type;
}
