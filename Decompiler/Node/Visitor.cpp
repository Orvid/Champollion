#include "Visitor.hpp"

#include <cassert>

#include "Nodes.hpp"

void Node::Visitor::visitChildren(Node::Base *node)
{
    assert(node);
    for (auto child : *node)
    {
        if (child)
        {
            child->visit(this);
        }
    }
}

#define DO_NODE(NODE) \
void Node::VisitorBase::visit(Node::##NODE *node) \
{ \
    assert(node); \
    visitChildren(node); \
}

FOR_EACH_NODE_CLASS()
#undef DO_NODE

Node::DynamicVisitor::DynamicVisitor()
{
    /*
    m_OnCommon = [&] (Base* node, DynamicVisitor* visitor)
    {
        visitor->VisitorBase::visit(node);
    };
    */
}

Node::DynamicVisitor &Node::DynamicVisitor::common(Node::DynamicVisitor::LambdaCommon common)
{
    m_OnCommon = common;
    return *this;
}


#define DO_NODE(NODE) \
void Node::DynamicVisitor::visit(Node::##NODE *node) \
{ \
    assert(node); \
    if (m_On##NODE) \
        m_On##NODE(node, this); \
    else if (m_OnCommon) \
        m_OnCommon(node, this); \
    else \
        VisitorBase::visit(node); \
}

FOR_EACH_NODE_CLASS()
#undef DO_NODE


#define DO_NODE(NODE) \
Node::DynamicVisitor& Node::DynamicVisitor::operator<<(Lambda##NODE function) \
{ \
    m_On##NODE = function; \
    return *this; \
}

FOR_EACH_NODE_CLASS()
#undef DO_NODE
