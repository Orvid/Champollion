#include "NodeComparer.hpp"

#include <cassert>

static bool isSameChildren(Node::Base* ref, Node::Base* node)
{
    if (ref->size() == node->size())
    {
        for (auto i = 0; i < ref->size(); ++i)
        {
            if (ref->operator[](i) != node->operator [](i) && !isSameTree(ref->operator[](i), node->operator [](i)))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Node::NodeComparer::visit(Node::Scope *node)
{
    m_Visitor << (DynamicVisitor::LambdaScope)[&](Scope* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::BinaryOperator *node)
{
    m_Visitor << (DynamicVisitor::LambdaBinaryOperator)[&](BinaryOperator* ref, DynamicVisitor*) {
        if (ref->getOperator() == node->getOperator())
        {
            m_Result = isSameChildren(ref, node);
        }
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::UnaryOperator *node)
{
    m_Visitor << (DynamicVisitor::LambdaUnaryOperator)[&](UnaryOperator* ref, DynamicVisitor*) {
        if(ref->getOperator() == node->getOperator())
        {
            m_Result = isSameChildren(ref, node);
        }
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::Assign *node)
{
    m_Visitor << (DynamicVisitor::LambdaAssign)[&](Assign* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::AssignOperator *node)
{
    m_Visitor << (DynamicVisitor::LambdaAssignOperator) [&](AssignOperator* ref, DynamicVisitor*) {
        if(ref->getOperator() == node->getOperator())
        {
            m_Result = isSameChildren(ref, node);
        }
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::Copy *node)
{
    m_Visitor << (DynamicVisitor::LambdaCopy)[&](Copy* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::Cast *node)
{
    m_Visitor << (DynamicVisitor::LambdaCast)[&](Cast* ref, DynamicVisitor*) {
        if(ref->getType() == node->getType())
        {
            m_Result = isSameChildren(ref, node);
        }
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::CallMethod *node)
{
    m_Visitor << (DynamicVisitor::LambdaCallMethod)[&](CallMethod* ref, DynamicVisitor*) {
        if (ref->getMethod() == node->getMethod())
        {
            m_Result = isSameChildren(ref, node);
        }
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::Params *node)
{
    m_Visitor << (DynamicVisitor::LambdaParams)[&](Params* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::Return *node)
{
    m_Visitor << (DynamicVisitor::LambdaReturn)[&](Return* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::PropertyAccess *node)
{
    m_Visitor << (DynamicVisitor::LambdaPropertyAccess)[&](PropertyAccess* ref, DynamicVisitor*) {
        if(ref->getProperty() == node->getProperty())
        {
            m_Result = isSameChildren(ref, node);
        }
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::StructCreate *node)
{
    m_Visitor << (DynamicVisitor::LambdaArrayCreate)[&](ArrayCreate* ref, DynamicVisitor*) {
        m_Result = ref->getType() == node->getType();
    };
    m_Reference->visit(&m_Visitor);
}

void Node::NodeComparer::visit(Node::ArrayCreate *node)
{
    m_Visitor << (DynamicVisitor::LambdaArrayCreate)[&](ArrayCreate* ref, DynamicVisitor*) {
        if (ref->getType() == node->getType())
        {
            m_Result = isSameChildren(ref, node);
        }
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::ArrayLength *node)
{
    m_Visitor << (DynamicVisitor::LambdaArrayLength)[&](ArrayLength* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::ArrayAccess *node)
{
    m_Visitor << (DynamicVisitor::LambdaArrayAccess)[&](ArrayAccess* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::Constant *node)
{
    m_Visitor << (DynamicVisitor::LambdaConstant)[&](Constant* ref, DynamicVisitor*) {
        m_Result = ref->getConstant() == node->getConstant();
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::IdentifierString *node)
{
    m_Visitor << (DynamicVisitor::LambdaIdentifierString)[&](IdentifierString* ref, DynamicVisitor*) {
        m_Result = ref->getIdentifier() == node->getIdentifier();
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::While *node)
{
    m_Visitor << (DynamicVisitor::LambdaWhile)[&](While* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::IfElse *node)
{
    m_Visitor << (DynamicVisitor::LambdaIfElse)[&](IfElse* ref, DynamicVisitor*) {
        m_Result = isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

void Node::NodeComparer::visit(Node::Declare *node)
{
    m_Visitor << (DynamicVisitor::LambdaDeclare)[&](Declare* ref, DynamicVisitor*) {
        m_Result = ref->getType() == node->getType() && isSameChildren(ref, node);
    };
    m_Reference->visit(& m_Visitor);
}

bool Node::isSameTree(Node::BasePtr left, Node::BasePtr right)
{
    NodeComparer comparer(left);
    right->visit(&comparer);
    return comparer.getResult();
}
