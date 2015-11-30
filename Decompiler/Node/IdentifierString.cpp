#include "IdentifierString.hpp"

#include <cassert>
#include "Visitor.hpp"

Node::IdentifierString::IdentifierString(size_t ip, const std::string &identifier) :
    Base(0, ip, 0),
    m_Identifier(identifier)
{
}

Node::IdentifierString::~IdentifierString()
{
}

std::shared_ptr<Node::IdentifierString> Node::IdentifierString::make(size_t ip, const std::string &identifier)
{
    return std::make_shared<Node::IdentifierString>(ip, identifier);
}

void Node::IdentifierString::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}


const std::string &Node::IdentifierString::getIdentifier() const
{
    return m_Identifier;
}
