#include "Params.hpp"

#include <cassert>

#include "Visitor.hpp"

Node::Params::Params() :
    Base(0, -1, 10)
{

}

Node::Params::~Params()
{
}

std::shared_ptr<Node::Params> Node::Params::make()
{
    return std::shared_ptr<Node::Params>(new Params());
}

void Node::Params::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

