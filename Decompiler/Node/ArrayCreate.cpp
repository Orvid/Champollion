#include "ArrayCreate.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::ArrayCreate::ArrayCreate(size_t ip, const Pex::StringTable::Index &result, const Pex::StringTable::Index &type, std::uint32_t size) :
    Base(0, ip, 0, result),
    m_Type(type),
    m_Size(size)
{
}

Node::ArrayCreate::~ArrayCreate()
{
}

std::shared_ptr<Node::ArrayCreate> Node::ArrayCreate::make(size_t ip, const Pex::StringTable::Index &result, const Pex::StringTable::Index &type, std::uint32_t size)
{
    return std::shared_ptr<ArrayCreate>(new ArrayCreate(ip, result, type, size));
}

void Node::ArrayCreate::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const Pex::StringTable::Index &Node::ArrayCreate::getType() const
{
    return m_Type;
}

std::uint32_t Node::ArrayCreate::getSize() const
{
    return m_Size;
}
