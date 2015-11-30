#include "Constant.hpp"


#include <cassert>

#include "Visitor.hpp"



Node::Constant::Constant(size_t ip, const Pex::Value &constant) :
    Base(0, ip, 0),
    m_Constant(constant)
{
}

Node::Constant::~Constant()
{
}

std::shared_ptr<Node::Constant> Node::Constant::make(size_t ip, const Pex::Value &constant)
{
    return std::shared_ptr<Node::Constant>(new Constant(ip,constant));
}

void Node::Constant::visit(Node::Visitor *visitor)
{
    assert(visitor);
    visitor->visit(this);
}

const Pex::Value &Node::Constant::getConstant() const
{
    return m_Constant;
}
