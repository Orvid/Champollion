#include "Scope.hpp"

#include <algorithm>
#include <cassert>

#include "Visitor.hpp"

Node::Scope::Scope() :
    Base(0, -1, 10)
{

}

Node::Scope::~Scope()
{
}

std::shared_ptr<Node::Scope> Node::Scope::make()
{
    return std::shared_ptr<Node::Scope>(new Scope());
}

void Node::Scope::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}
