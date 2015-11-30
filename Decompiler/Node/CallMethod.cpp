#include "CallMethod.hpp"

#include <cassert>
#include "Visitor.hpp"

#include "Params.hpp"

Node::CallMethod::CallMethod(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr &object, const Pex::StringTable::Index &method) :
    Base(2, ip, 0, result),
    FieldObjectNodeMixin(this, object),
    FieldParametersNodeMixin(this, Params::make()),
    m_Method(method)
{
}

Node::CallMethod::~CallMethod()
{
}

std::shared_ptr<Node::CallMethod> Node::CallMethod::make(size_t ip, const Pex::StringTable::Index &result, Node::BasePtr object, const Pex::StringTable::Index &method)
{
    return std::shared_ptr<Node::CallMethod>(new CallMethod(ip, result, object, method));
}

void Node::CallMethod::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const Pex::StringTable::Index &Node::CallMethod::getMethod() const
{
    return m_Method;
}
