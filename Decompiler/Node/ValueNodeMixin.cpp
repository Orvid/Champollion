#include "ValueNodeMixin.hpp"

#include <cassert>

Node::ValueNodeMixin::ValueNodeMixin(Node::BasePtr value) :
    m_Value(value)
{
    assert(value);
}

Node::BasePtr Node::ValueNodeMixin::getValue() const
{
    return m_Value;
}

void Node::ValueNodeMixin::setValue(Node::BasePtr value)
{
    assert(value);
    m_Value = value;
}
