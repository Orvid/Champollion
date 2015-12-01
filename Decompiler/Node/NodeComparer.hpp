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

    void visit(Node::Scope* node) override;
    void visit(Node::BinaryOperator* node) override;
    void visit(Node::UnaryOperator* node) override;
    void visit(Node::Assign* node) override;
    void visit(Node::AssignOperator* node) override;
    void visit(Node::Copy* node) override;
    void visit(Node::Cast* node) override;
    void visit(Node::CallMethod* node) override;
    void visit(Node::Params* node) override;
    void visit(Node::Return* node) override;
    void visit(Node::PropertyAccess* node) override;
    void visit(Node::StructCreate* node) override;
    void visit(Node::ArrayCreate* node) override;
    void visit(Node::ArrayLength* node) override;
    void visit(Node::ArrayAccess* node) override;
    void visit(Node::Constant* node) override;
    void visit(Node::IdentifierString* node) override;
    void visit(Node::While* node) override;
    void visit(Node::IfElse* node) override;
    void visit(Node::Declare* node) override;

private:
    BasePtr m_Reference;
    DynamicVisitor m_Visitor;
    bool m_Result;
};

bool isSameTree(BasePtr left, BasePtr right);

}
