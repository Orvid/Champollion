#pragma once

#include "Base.hpp"
#include "Nodes.hpp"
#include "Visitor.hpp"

namespace Node {

class NodeComparer final : public Visitor
{
public:
    NodeComparer(BasePtr reference) :
        m_Reference(reference),
        m_Result(false)
    {
        m_Visitor.common([&](Base*, DynamicVisitor*) {});
    }

    virtual ~NodeComparer() = default;

    bool getResult() const { return m_Result; }

    virtual void visit(Node::Scope* node);
    virtual void visit(Node::BinaryOperator* node);
    virtual void visit(Node::UnaryOperator* node);
    virtual void visit(Node::Assign* node);
    virtual void visit(Node::AssignOperator* node);
    virtual void visit(Node::Copy* node);
    virtual void visit(Node::Cast* node);
    virtual void visit(Node::CallMethod* node);
    virtual void visit(Node::Params* node);
    virtual void visit(Node::Return* node);
    virtual void visit(Node::PropertyAccess* node);
    virtual void visit(Node::ArrayCreate* node);
    virtual void visit(Node::ArrayLength* node);
    virtual void visit(Node::ArrayAccess* node);
    virtual void visit(Node::Constant* node);
    virtual void visit(Node::IdentifierString* node);
    virtual void visit(Node::While* node);
    virtual void visit(Node::IfElse* node);
    virtual void visit(Node::Declare* node);

protected:
    BasePtr m_Reference;
    DynamicVisitor m_Visitor;
    bool m_Result;
};

bool isSameTree(BasePtr left, BasePtr right);

}
