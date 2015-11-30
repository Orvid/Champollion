#include "ArrayMixin.hpp"

#include <cassert>

Node::ArrayMixin::ArrayMixin(Node::BasePtr array) :
    m_Array(array)
{
    assert(array);
}

Node::BasePtr Node::ArrayMixin::getArray() const
{
    return m_Array;
}

void Node::ArrayMixin::setArray(Node::BasePtr array)
{
    assert(array);
    m_Array = array;
}
